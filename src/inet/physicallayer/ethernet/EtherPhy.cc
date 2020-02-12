//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/ethernet/EtherPhyFrame_m.h"
#include "inet/physicallayer/ethernet/EtherPhy.h"

namespace inet {

namespace physicallayer {

Define_Module(EtherPhy);

simsignal_t EtherPhy::txStateChangedSignal = registerSignal("txStateChanged");
simsignal_t EtherPhy::rxStateChangedSignal = registerSignal("rxStateChanged");

void EtherPhy::initialize(int stage)
{
    cPhyModule::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        physInGate = gate("phys$i");
        physOutGate = gate("phys$o");
        upperLayerInGate = gate("upperLayerIn");
        bitrate = par("bitrate");
        duplexMode = par("duplexMode");
        if (!duplexMode)
            throw cRuntimeError("half-duplex currently not supported.");
        displayStringTextFormat = par("displayStringTextFormat");
        sendRawBytes = par("sendRawBytes");

        // initialize connected flag
        connected = physOutGate->getPathEndGate()->isConnected() && physInGate->getPathStartGate()->isConnected();
        if (!connected)
            EV_WARN << "PHY not connected to a network.\n";

        // initialize states
        txState = connected ? TX_IDLE_STATE : TX_OFF_STATE;
        rxState = connected ? RX_IDLE_STATE : RX_OFF_STATE;

        // initialize self messages
        endTxMsg = new cMessage("EndTransmission", ENDTRANSMISSION);
    }
    else if (stage == INITSTAGE_NETWORK_INTERFACE_CONFIGURATION) {
        interfaceEntry = getContainingNicModule(this);
    }
    else if (stage == INITSTAGE_LINK_LAYER) {
        cDatarateChannel *outTrChannel = dynamic_cast<cDatarateChannel *>(physOutGate->findTransmissionChannel());
        if (outTrChannel != nullptr)
            bitrate = outTrChannel->getDatarate();
    }
}

void EtherPhy::changeTxState(TxState newState)
{
    if (newState != txState) {
        txState = newState;
        emit(txStateChangedSignal, newState);
    }
}

void EtherPhy::handleMessage(cMessage *message)
{
    if (message->isSelfMessage()) {
        if (message == endTxMsg) {
            changeTxState(TX_IDLE_STATE);
        }
        else
            throw cRuntimeError("Unknown self message received!");
    }
    else if (message->getArrivalGate() == upperLayerInGate) {
        auto packet = check_and_cast<Packet *>(message);
        auto phyHeader = makeShared<EthernetPhyHeader>();
        packet->insertAtFront(phyHeader);
        packet->clearTags();
        packet->addTag<PacketProtocolTag>()->setProtocol(&Protocol::ethernetPhy);
        auto signal = new EthernetSignal(packet->getName());
        signal->setSrcMacFullDuplex(duplexMode);
        signal->setBitrate(bitrate);
        signal->encapsulate(packet);
        send(signal, physOutGate);
        scheduleAt(transmissionChannel->getTransmissionFinishTime(), endTxMsg);
        changeTxState(TX_TRANSMITTING_STATE);
    }
    else if (message->getArrivalGate() == physInGate) {
        auto signal = check_and_cast<EthernetSignalBase *>(message);
        if (signal->getSrcMacFullDuplex() != duplexMode)
            throw cRuntimeError("Ethernet misconfiguration: MACs on the same link must be all in full duplex mode, or all in half-duplex mode");
        if (signal->getBitrate() != bitrate)
            throw cRuntimeError("Ethernet misconfiguration: MACs on the same link must be same bitrate");
        auto packet = check_and_cast<Packet *>(signal->decapsulate());
        delete signal;
        auto phyHeader = packet->popAtFront<EthernetPhyHeader>();
        packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::ethernetMac);
        send(packet, "upperLayerOut");
    }
    else
        throw cRuntimeError("Received unknown message");
}

} // namespace physicallayer

} // namespace inet

