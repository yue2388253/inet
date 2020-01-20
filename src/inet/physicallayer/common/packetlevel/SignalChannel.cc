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
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/physicallayer/common/packetlevel/SignalChannel.h"

namespace inet {
namespace physicallayer {

Register_Class(SignalChannel);

SignalChannel::SignalChannel(const char *name) : cDatarateChannel(name)
{
}

void SignalChannel::initialize()
{
    cDatarateChannel::initialize();
    // TODO:
}

void SignalChannel::processMessage(cMessage *msg, simtime_t t, result_t& result)
{
    if (dynamic_cast<physicallayer::Signal *>(msg)) {
        if (lastSignal != nullptr)
            throw cRuntimeError("Unfinished transmission: (%s)%s,t=%s at %s", lastSignal->getClassName(), lastSignal->getFullName(), lastSignalStartTime.str().c_str(), t.str().c_str());

        cDatarateChannel::processMessage(msg, t, result);
    }
    else if (auto signalStart = dynamic_cast<physicallayer::SignalStart *>(msg)) {
        if (lastSignal != nullptr)
            throw cRuntimeError("Unfinished transmission: (%s)%s,t=%s at %s", lastSignal->getClassName(), lastSignal->getFullName(), lastSignalStartTime.str().c_str(), t.str().c_str());

        //TODO based on copy-paste from cDatarateChannel

        // channel must be idle
        if (txFinishTime > t)
            throw cRuntimeError("Cannot send message (%s)%s on gate %s: Channel is currently "
                                "busy with an ongoing transmission -- please rewrite the sender "
                                "simple module to only send when the previous transmission has "
                                "already finished, using cGate::getTransmissionFinishTime(), scheduleAt(), "
                                "and possibly a cQueue for storing messages waiting to be transmitted",
                    msg->getClassName(), msg->getFullName(), this->getSourceGate()->getFullPath().c_str());

        if (getTransmissionFinishTime() != -1 && mayHaveListeners(channelBusySignal)) {
            cTimestampedValue tmp(getTransmissionFinishTime(), (intval_t)0);
            emit(channelBusySignal, &tmp);
        }

        auto *signal = signalStart->getSignal();
        lastSignal = signal;
        lastSignalStartTime = t;

        // datarate modeling
        simtime_t duration = calculateDuration(const_cast<physicallayer::Signal *>(signal));
        // result.duration = duration; // Do not set duration!
        txFinishTime = t + duration;

        // propagation delay modeling
        result.delay = delay;

        // if channel is disabled, signal that message should be deleted
        if (isDisabled()) {
            result.discard = true;
            cTimestampedValue tmp(t, msg);
            emit(messageDiscardedSignal, &tmp);
            return;
        }

        // bit error modeling REMOVED!

        // emit signals
        if (mayHaveListeners(channelBusySignal)) {
            cTimestampedValue tmp(t, (intval_t)1);
            emit(channelBusySignal, &tmp);
        }
    }

        if (lastSignalEndTime < t)
            throw cRuntimeError("The SignalEnd was come too later");

    else if (auto signalEnd = dynamic_cast<physicallayer::SignalEnd *>(msg)) {
        auto *signal = signalEnd->getSignal();
        if (lastSignal == nullptr || lastSignalStartTime == -1)
            throw cRuntimeError("SignalStart was missed");

        if (lastSignal->getTreeId() != signal->getTreeId())
            throw cRuntimeError("SignalEnd does not match for SignalStart");

        if (txFinishTime < t)
            throw cRuntimeError("The SignalEnd arrived too later");

        simtime_t duration = calculateDuration(signal);

        if (t != lastSignalStartTime + duration)
            throw cRuntimeError("SignalEnd duration error");    //TODO a signalend-ben lévő signal duration-je nem egyezik meg a signalstart óta eltelt idővel.
        signal->setDuration(duration);

        lastSignal = nullptr;
        lastSignalStartTime = -1;
        txFinishTime = t;

        //TODO based on copy-paste from cDatarateChannel

        // if channel is disabled, signal that message should be deleted
        if (flags & FL_ISDISABLED) {
            result.discard = true;
            cTimestampedValue tmp(t, msg);
            emit(messageDiscardedSignal, &tmp);
            return;
        }

        // datarate modeling

        // propagation delay modeling
        result.delay = getDelay();

        // bit error modeling
        if ((flags & (FL_BER_NONZERO | FL_PER_NONZERO)) && msg->isPacket()) {
            if (flags & FL_BER_NONZERO)
                if (dblrand() < 1.0 - pow(1.0 - ber, (double)signal->getBitLength()))
                    signal->setBitError(true);

            if (flags & FL_PER_NONZERO)
                if (dblrand() < per)
                    signal->setBitError(true);
        }

        // emit signals
        if (mayHaveListeners(messageSentSignal)) {
            MessageSentSignalValue tmp(t, msg, &result);
            emit(messageSentSignal, &tmp);
        }
        if (mayHaveListeners(channelBusySignal)) {
            cTimestampedValue tmp(t, (intval_t)0);
            emit(channelBusySignal, &tmp);
        }
    }
    else
        throw cRuntimeError("Unknown message type: '%s'", msg->getClassName());
}

} // namespace physicallayer
} // namespace inet

