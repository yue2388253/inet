//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#ifndef __INET_FLOWMEASUREMENTRECORDER_H
#define __INET_FLOWMEASUREMENTRECORDER_H

#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/packet/PacketFilter.h"
#include "inet/queueing/base/PacketFlowBase.h"

namespace inet {
namespace queueing {

class INET_API FlowMeasurementRecorder : public PacketFlowBase, public TransparentProtocolRegistrationListener
{
  public:
    static simsignal_t packetFlowMeasuredSignal;

  protected:
    PacketFilter packetFilter;
    b offset = b(0);
    b length = b(-1);
    bool endMeasurement_ = false;
    const char *flowName = nullptr;
    cMatchExpression flowNameMatcher;
    bool measureLifeTime = false;
    bool measureElapsedTime = false;
    bool measureDelayingTime = false;
    bool measureQueueingTime = false;
    bool measureProcessingTime = false;
    bool measureTransmissionTime = false;
    bool measurePropagationTime = false;

  protected:
    virtual void initialize(int stage) override;
    virtual void makeMeasurement(Packet *packet, b offset, b length, const char *flowName, simsignal_t bitSignal, simsignal_t bitPerRegionSignal, simsignal_t packetPerBitSignal, simsignal_t packetPerRegionSignal, simtime_t bitValue, simtime_t packetValue);
    virtual void makeMeasurements(Packet *packet);
    virtual void endMeasurements(Packet *packet);

    virtual cGate *getRegistrationForwardingGate(cGate *gate) override;
    virtual bool isForwardingService(const Protocol& protocol, cGate *gate, ServicePrimitive servicePrimitive) const override { return false; }

    template<typename T>
    void makeMeasurement(Packet *packet, b offset, b length, simsignal_t bitSignal, simsignal_t bitPerRegionSignal, simsignal_t packetPerBitSignal, simsignal_t packetPerRegionSignal) {
        packet->mapAllRegionTags<T>(offset, length, [&] (b o, b l, const Ptr<const T>& timeTag) {
            for (int i = 0; i < (int)timeTag->getBitTotalTimesArraySize(); i++) {
                auto flowName = timeTag->getFlowNames(i);
                cMatchableString matchableFlowName(flowName);
                if (flowNameMatcher.matches(&matchableFlowName))
                    makeMeasurement(packet, o, l, flowName, bitSignal, bitPerRegionSignal, packetPerBitSignal, packetPerRegionSignal, timeTag->getBitTotalTimes(i), timeTag->getPacketTotalTimes(i));
            }
        });
    }

    template<typename T>
    void endMeasurement(Packet *packet, b offset, b length) {
        packet->mapAllRegionTagsForUpdate<T>(offset, length, [&] (b o, b l, const Ptr<T>& timeTag) {
            for (int i = 0; i < (int)timeTag->getBitTotalTimesArraySize(); i++) {
                auto flowName = timeTag->getFlowNames(i);
                cMatchableString matchableFlowName(flowName);
                if (flowNameMatcher.matches(&matchableFlowName)) {
                    EV_INFO << "Stopping measurement on packet" << EV_FIELD(offset) << EV_FIELD(length);
                    if (flowName != nullptr && *flowName != '\0')
                        EV_INFO << EV_FIELD(flowName);
                    EV_INFO << EV_FIELD(packet) << EV_ENDL;
                    timeTag->eraseFlowNames(i);
                    timeTag->eraseBitTotalTimes(i);
                    timeTag->erasePacketTotalTimes(i);
                    i--;
                    break;
                }
            }
        });
    }

  public:
    virtual void processPacket(Packet *packet) override;
};

} // namespace queueing
} // namespace inet

#endif

