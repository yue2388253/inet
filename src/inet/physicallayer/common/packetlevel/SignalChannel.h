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

#ifndef __INET_SIGNALCHANNEL_H
#define __INET_SIGNALCHANNEL_H

#include "inet/common/INETDefs.h"
#include "inet/physicallayer/common/packetlevel/Signal.h"

namespace inet {
namespace physicallayer {

// TODO: rename?
class INET_API SignalChannel : public cDatarateChannel
{
  protected:
    const physicallayer::Signal *lastSignal = nullptr;
    simtime_t lastSignalStartTime = -1;
  public:
    SignalChannel(const char *name = nullptr);

    virtual void initialize() override;
    virtual simtime_t calculateDuration(cMessage *msg) const override;
    virtual bool isBusy() const override { return lastSignal != nullptr || simTime() < txFinishTime; }
    virtual void processMessage(cMessage *msg, simtime_t t, result_t& result) override;     // accepts only Signal, SignalStart, SignalChange, SignalEnd msg-s
};

} // namespace physicallayer
} // namespace inet

#endif // ifndef __INET_SIGNALCHANNEL_H

