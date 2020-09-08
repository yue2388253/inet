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
#include "inet/tracker/base/LinkTrackerBase.h"

namespace inet {

namespace tracker {

LinkTrackerBase::~LinkTrackerBase()
{
    if (trackLinks)
        unsubscribe();
}

void LinkTrackerBase::initialize(int stage)
{
    TrackerBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        trackLinks = par("trackLinks");
        const char *activityLevelString = par("activityLevel");
        if (!strcmp(activityLevelString, "service"))
            activityLevel = ACTIVITY_LEVEL_SERVICE;
        else if (!strcmp(activityLevelString, "peer"))
            activityLevel = ACTIVITY_LEVEL_PEER;
        else if (!strcmp(activityLevelString, "protocol"))
            activityLevel = ACTIVITY_LEVEL_PROTOCOL;
        else
            throw cRuntimeError("Unknown activity level: %s", activityLevelString);
        nodeFilter.setPattern(par("nodeFilter"));
        interfaceFilter.setPattern(par("interfaceFilter"));
        packetFilter.setPattern(par("packetFilter"), par("packetDataFilter"));
        if (trackLinks)
            subscribe();
    }
}

void LinkTrackerBase::handleParameterChange(const char *name)
{
    if (name != nullptr) {
        if (!strcmp(name, "nodeFilter"))
            nodeFilter.setPattern(par("nodeFilter"));
        else if (!strcmp(name, "interfaceFilter"))
            interfaceFilter.setPattern(par("interfaceFilter"));
        else if (!strcmp(name, "packetFilter") || !strcmp(name, "packetDataFilter"))
            packetFilter.setPattern(par("packetFilter"), par("packetDataFilter"));
    }
}

void LinkTrackerBase::subscribe()
{
    if (activityLevel == ACTIVITY_LEVEL_SERVICE) {
        trackingSubjectModule->subscribe(packetSentToUpperSignal, this);
        trackingSubjectModule->subscribe(packetReceivedFromUpperSignal, this);
    }
    else if (activityLevel == ACTIVITY_LEVEL_PEER) {
        trackingSubjectModule->subscribe(packetSentToPeerSignal, this);
        trackingSubjectModule->subscribe(packetReceivedFromPeerSignal, this);
    }
    else if (activityLevel == ACTIVITY_LEVEL_PROTOCOL) {
        trackingSubjectModule->subscribe(packetSentToLowerSignal, this);
        trackingSubjectModule->subscribe(packetReceivedFromLowerSignal, this);
    }
}

void LinkTrackerBase::unsubscribe()
{
    // NOTE: lookup the module again because it may have been deleted first
    auto trackingSubjectModule = findModuleFromPar<cModule>(par("trackingSubjectModule"), this);
    if (trackingSubjectModule != nullptr) {
        if (activityLevel == ACTIVITY_LEVEL_SERVICE) {
            trackingSubjectModule->unsubscribe(packetSentToUpperSignal, this);
            trackingSubjectModule->unsubscribe(packetReceivedFromUpperSignal, this);
        }
        else if (activityLevel == ACTIVITY_LEVEL_PEER) {
            trackingSubjectModule->unsubscribe(packetSentToPeerSignal, this);
            trackingSubjectModule->unsubscribe(packetReceivedFromPeerSignal, this);
        }
        else if (activityLevel == ACTIVITY_LEVEL_PROTOCOL) {
            trackingSubjectModule->unsubscribe(packetSentToLowerSignal, this);
            trackingSubjectModule->unsubscribe(packetReceivedFromLowerSignal, this);
        }
    }
}

cModule *LinkTrackerBase::getLastModule(int treeId)
{
    auto it = lastModules.find(treeId);
    if (it == lastModules.end())
        return nullptr;
    else
        return getSimulation()->getModule(it->second);
}

void LinkTrackerBase::setLastModule(int treeId, cModule *module)
{
    lastModules[treeId] = module->getId();
}

void LinkTrackerBase::removeLastModule(int treeId)
{
    lastModules.erase(lastModules.find(treeId));
}

void LinkTrackerBase::receiveSignal(cComponent *source, simsignal_t signal, cObject *object, cObject *details)
{
    Enter_Method_Silent();
    if ((activityLevel == ACTIVITY_LEVEL_SERVICE && signal == packetReceivedFromUpperSignal) ||
        (activityLevel == ACTIVITY_LEVEL_PEER && signal == packetSentToPeerSignal) ||
        (activityLevel == ACTIVITY_LEVEL_PROTOCOL && signal == packetSentToLowerSignal))
    {
        if (isLinkStart(static_cast<cModule *>(source))) {
            auto module = check_and_cast<cModule *>(source);
            auto packet = check_and_cast<Packet *>(object);
            mapChunks(packet->peekAt(b(0), packet->getTotalLength()), [&] (const Ptr<const Chunk>& chunk, int id) { if (getLastModule(id) != nullptr) removeLastModule(id); });
            auto networkNode = getContainingNode(module);
            auto networkInterface = getContainingNicModule(module);
            if (nodeFilter.matches(networkNode) && interfaceFilter.matches(networkInterface) && packetFilter.matches(packet)) {
                mapChunks(packet->peekAt(b(0), packet->getTotalLength()), [&] (const Ptr<const Chunk>& chunk, int id) { setLastModule(id, module); });
            }
        }
    }
    else if ((activityLevel == ACTIVITY_LEVEL_SERVICE && signal == packetSentToUpperSignal) ||
             (activityLevel == ACTIVITY_LEVEL_PEER && signal == packetReceivedFromPeerSignal) ||
             (activityLevel == ACTIVITY_LEVEL_PROTOCOL && signal == packetReceivedFromLowerSignal))
    {
        if (isLinkEnd(static_cast<cModule *>(source))) {
            auto module = check_and_cast<cModule *>(source);
            auto packet = check_and_cast<Packet *>(object);
            auto networkNode = getContainingNode(module);
            auto networkInterface = getContainingNicModule(module);
            if (nodeFilter.matches(networkNode) && interfaceFilter.matches(networkInterface) && packetFilter.matches(packet)) {
                cModule *senderModule = nullptr;
                mapChunks(packet->peekAt(b(0), packet->getTotalLength()), [&] (const Ptr<const Chunk>& chunk, int id) {
                    auto lastModule = getLastModule(id);
                    if (lastModule != nullptr)
                        senderModule = lastModule;
                    // NOTE: don't call removeLastModule(treeId) because other network nodes may still receive this packet
                });
                if (senderModule != nullptr) {
                    auto trackedPacket = packet->dup();
                    trackedPacket->trim();
                    auto senderGate = senderModule->gate("upperLayerIn"); // TODO:
                    trackedPacket->setSentFrom(senderModule, senderGate->getId(), simTime());
                    auto envir = getEnvir();
                    envir->beginSend(trackedPacket, SendOptions().tags(getTags()));
                    auto arrivalGate = module->gate("upperLayerOut"); // TODO:
                    envir->messageSendDirect(trackedPacket, arrivalGate, ChannelResult());
                    envir->endSend(trackedPacket);
                    // TODO: EV_INFO << "Virtual packet send" << EV_FIELD(sender, senderModule) << EV_FIELD(receiver, module) << EV_FIELD(packet, trackedPacket) << EV_ENDL;
                    delete trackedPacket;
                }
            }
        }
    }
    else
        throw cRuntimeError("Unknown signal");
}

} // namespace tracker

} // namespace inet

