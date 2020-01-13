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
namespace physicallyer {

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
    cDatarateChannel::processMessage(msg, t, result);
    // TODO:
}

} // namespace physicallayer
} // namespace inet

