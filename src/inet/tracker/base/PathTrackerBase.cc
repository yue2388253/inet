//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/tracker/base/PathTrackerBase.h"

namespace inet {

namespace tracker {

PathTrackerBase::~PathTrackerBase()
{
    if (trackPaths)
        unsubscribe();
}

void PathTrackerBase::initialize(int stage)
{
    TrackerBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        trackPaths = par("trackPaths");
        nodeFilter.setPattern(par("nodeFilter"));
        packetFilter.setPattern(par("packetFilter"), par("packetDataFilter"));
        if (trackPaths)
            subscribe();
    }
}

void PathTrackerBase::handleParameterChange(const char *name)
{
    if (name != nullptr) {
        if (!strcmp(name, "nodeFilter"))
            nodeFilter.setPattern(par("nodeFilter"));
        else if (!strcmp(name, "packetFilter"))
            packetFilter.setPattern(par("packetFilter"), par("packetDataFilter"));
    }
}

void PathTrackerBase::subscribe()
{
    trackingSubjectModule->subscribe(packetSentToUpperSignal, this);
    trackingSubjectModule->subscribe(packetReceivedFromUpperSignal, this);
    trackingSubjectModule->subscribe(packetReceivedFromLowerSignal, this);
}

void PathTrackerBase::unsubscribe()
{
    // NOTE: lookup the module again because it may have been deleted first
    auto trackingSubjectModule = findModuleFromPar<cModule>(par("trackingSubjectModule"), this);
    if (trackingSubjectModule != nullptr) {
        trackingSubjectModule->unsubscribe(packetSentToUpperSignal, this);
        trackingSubjectModule->unsubscribe(packetReceivedFromUpperSignal, this);
        trackingSubjectModule->unsubscribe(packetReceivedFromLowerSignal, this);
    }
}

const std::vector<int> *PathTrackerBase::getIncompletePath(int chunkId)
{
    auto it = incompletePaths.find(chunkId);
    if (it == incompletePaths.end())
        return nullptr;
    else
        return &it->second;
}

void PathTrackerBase::addToIncompletePath(int chunkId, cModule *module)
{
    auto& moduleIds = incompletePaths[chunkId];
    auto moduleId = module->getId();
    if (moduleIds.size() == 0 || moduleIds[moduleIds.size() - 1] != moduleId)
        moduleIds.push_back(moduleId);
}

void PathTrackerBase::removeIncompletePath(int chunkId)
{
    incompletePaths.erase(incompletePaths.find(chunkId));
}

void PathTrackerBase::receiveSignal(cComponent *source, simsignal_t signal, cObject *object, cObject *details)
{
    Enter_Method_Silent();
    if (signal == packetReceivedFromUpperSignal) {
        if (isPathStart(static_cast<cModule *>(source))) {
            auto module = check_and_cast<cModule *>(source);
            auto networkNode = getContainingNode(module);
            auto packet = check_and_cast<Packet *>(object);
            if (nodeFilter.matches(networkNode) && packetFilter.matches(packet)) {
                mapChunks(packet->peekAt(b(0), packet->getTotalLength()), [&] (const Ptr<const Chunk>& chunk, int id) {
                    auto path = getIncompletePath(id);
                    if (path != nullptr)
                        removeIncompletePath(id);
                    addToIncompletePath(id, module);
                });
            }
        }
    }
    else if (signal == packetReceivedFromLowerSignal) {
        if (isPathElement(static_cast<cModule *>(source))) {
            auto module = check_and_cast<cModule *>(source);
            auto packet = check_and_cast<Packet *>(object);
            if (packetFilter.matches(packet)) {
                mapChunks(packet->peekAt(b(0), packet->getTotalLength()), [&] (const Ptr<const Chunk>& chunk, int id) {
                    auto path = getIncompletePath(id);
                    if (path != nullptr)
                        addToIncompletePath(id, module);
                });
            }
        }
    }
    else if (signal == packetSentToUpperSignal) {
        if (isPathEnd(static_cast<cModule *>(source))) {
            auto module = check_and_cast<cModule *>(source);
            auto networkNode = getContainingNode(module);
            auto packet = check_and_cast<Packet *>(object);
            if (nodeFilter.matches(networkNode) && packetFilter.matches(packet)) {
                mapChunks(packet->peekAt(b(0), packet->getTotalLength()), [&] (const Ptr<const Chunk>& chunk, int id) {
                    auto path = getIncompletePath(id);
                    if (path != nullptr) {
                        if (path->size() > 1) {
                            auto senderModule = check_and_cast<cModule *>(getSimulation()->getComponent(path->at(0)));
                            auto trackedPacket = packet->dup();
                            trackedPacket->trim();
                            auto senderGate = senderModule->hasGate("transportIn") ? senderModule->gate("transportIn") : senderModule->gate("appIn"); // TODO:
                            trackedPacket->setSentFrom(senderModule, senderGate->getId(), simTime());
                            auto envir = getEnvir();
                            envir->beginSend(trackedPacket, SendOptions().tags(getTags()));
                            auto arrivalGate = module->hasGate("transportOut") ? module->gate("transportOut") : module->gate("appOut"); // TODO:
                            envir->messageSendDirect(trackedPacket, arrivalGate, ChannelResult());
                            envir->endSend(trackedPacket);
                            // TODO: EV_INFO << "Virtual packet send" << EV_FIELD(sender, senderModule) << EV_FIELD(receiver, module) << EV_FIELD(packet, trackedPacket) << EV_ENDL;
                            delete trackedPacket;
                        }
                        removeIncompletePath(id);
                    }
                });
            }
        }
    }
    else
        throw cRuntimeError("Unknown signal");
}

} // namespace tracker

} // namespace inet

