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

#ifndef __INET_SIGNAL_H
#define __INET_SIGNAL_H

#include "inet/common/packet/Packet.h"

namespace inet {
namespace physicallayer {

class INET_API Signal : public cPacket
{
  public:
    explicit Signal(const char *name=nullptr, short kind=0, int64_t bitLength=0);
    Signal(const Signal& other);

    virtual Signal *dup() const override { return new Signal(*this); }
    virtual const Packet *getPacket() { return check_and_cast<Packet *>(getEncapsulatedPacket()); }
};

class INET_API SignalStart : public cMessage
{
  protected:
    const Signal *signal = nullptr;

  public:
    explicit SignalStart(const Signal *signal) : cMessage(signal->getName(),signal->getKind()), signal(signal) { /* do not change the ownership of signal */ }

    virtual SignalStart *dup() const override { return new SignalStart(*this); }
    virtual const Signal *getSignal() const { return signal; }
};

class INET_API SignalEnd : public cMessage
{
  protected:
    Signal *signal = nullptr;

  public:
    explicit SignalEnd(Signal *signal) : cMessage(signal->getName(),signal->getKind()), signal(signal) { /* TODO 'signal' ownership */ }

    virtual SignalEnd *dup() const override { return new SignalEnd(*this); }
    virtual Signal *getSignal() const { return signal; }
    //TODO get Signal with ownership
};

} // namespace physicallayer
} // namespace inet

#endif // ifndef __INET_SIGNAL_H

