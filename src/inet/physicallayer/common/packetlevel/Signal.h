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
  protected:
    simtime_t requestedDuration;
  public:
    explicit Signal(const char *name=nullptr, short kind=0, int64_t bitLength=0);
    Signal(const Signal& other);

    virtual Signal *dup() const override { return new Signal(*this); }
    virtual const Packet *getPacket() { return check_and_cast<Packet *>(getEncapsulatedPacket()); }
    virtual simtime_t getRequestedDuration() const { return requestedDuration; }
    virtual void setRequestedDuration(simtime_t d) { requestedDuration = d; }
};

class INET_API SignalStart : public cMessage
{
  protected:
    const Signal *signal = nullptr;

  public:
    explicit SignalStart(const Signal *signal) : cMessage((std::string(signal->getName())+"-start").c_str(), signal->getKind()), signal(signal) { /* do not change the ownership of signal */ }

    virtual SignalStart *dup() const override { return new SignalStart(*this); }
    virtual const Signal *getSignal() const { return signal; }
};

class INET_API SignalChange : public cMessage
{
  protected:
    const Signal *signal = nullptr;

  public:
    explicit SignalChange(const Signal *signal) : cMessage((std::string(signal->getName())+"-change").c_str(), signal->getKind()), signal(signal) { /* do not change the ownership of signal */ }

    virtual SignalChange *dup() const override { return new SignalChange(*this); }
    virtual const Signal *getSignal() const { return signal; }
};

class INET_API SignalEnd : public cMessage
{
  protected:
    Signal *signal = nullptr;

  public:
    explicit SignalEnd(Signal *signal) : cMessage((std::string(signal->getName())+"-end").c_str(), signal->getKind()), signal(signal) { take(this->signal); }

    virtual SignalEnd *dup() const override { auto se = new SignalEnd(*this); se->signal = signal->dup(); se->take(se->signal); return se; }
    virtual Signal *getSignal() const { return signal; }
    virtual Signal *removeSignal() { Signal *s = signal; signal = nullptr; drop(s); return s; }
    //TODO get Signal with ownership
};

} // namespace physicallayer
} // namespace inet

#endif // ifndef __INET_SIGNAL_H

