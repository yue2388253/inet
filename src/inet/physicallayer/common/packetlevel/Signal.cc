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

#include "inet/physicallayer/common/packetlevel/Signal.h"

namespace inet {
namespace physicallayer {

Signal::Signal(const char *name, short kind, int64_t bitLength) :
    cPacket(name, kind, bitLength)
{
}

Signal::Signal(const Signal& other) :
    cPacket(other)
{
}

uint64_t Signal::getAvailableBitLengthAt(simtime_t elapsedTime) const     // linear calculation based on bitLength, requestedDuration, elapsedTime
{
    if (elapsedTime >= getRequestedDuration())
        return getBitLength();
    return (uint64_t)(getBitLength() * (elapsedTime / getRequestedDuration()));   //TODO calculate it more precise
}

simtime_t Signal::getDurationOfFirstNBits(int64_t bits) const     // linear calculation based on bitLength, requestedDuration, bits
{
    ASSERT(bits >= 0 && bits <= getBitLength());
    if (bits == getBitLength())
        return getRequestedDuration();

    return (double)bits / getBitLength() * getRequestedDuration();   //TODO calculate it more precise
}

} // namespace physicallayer
} // namespace inet


