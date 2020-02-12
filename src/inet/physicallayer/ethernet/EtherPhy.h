//
// Copyright (C) OpenSimLtd.
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

#ifndef __INET_ETHERPHY_H
#define __INET_ETHERPHY_H

#include "inet/common/INETDefs.h"
#include "inet/networklayer/common/InterfaceEntry.h"

namespace inet {
namespace physicallayer {

class INET_API EtherPhy : public cPhyModule
{
  public:
    enum TxState {
        TX_OFF_STATE = 0,
        TX_IDLE_STATE,
        TX_TRANSMITTING_STATE,
    };

    enum RxState {
        RX_OFF_STATE = 0,
        RX_IDLE_STATE,
        RX_RECEIVING_STATE,
    };

  protected:
    // Self-message kind values
    enum SelfMsgKindValues {
        ENDTRANSMISSION = 101,
    };

    const char *displayStringTextFormat = nullptr;
    InterfaceEntry *interfaceEntry = nullptr;   // NIC module
    cChannel *transmissionChannel = nullptr;    // transmission channel
    cGate *physInGate = nullptr;    // pointer to the "phys$i" gate
    cGate *physOutGate = nullptr;    // pointer to the "phys$o" gate
    cGate *upperLayerInGate = nullptr;
    cMessage *endTxMsg = nullptr;
    double bitrate = NaN;
    bool   sendRawBytes = false;
    bool   duplexMode = true;
    bool   connected = false;    // true if connected to a network, set automatically by exploring the network configuration
    TxState txState = TX_OFF_STATE;    // "transmit state" of the MAC
    RxState rxState = RX_OFF_STATE;    // "receive state" of the MAC

    static simsignal_t txStateChangedSignal;
    static simsignal_t rxStateChangedSignal;

  public:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;

    void changeTxState(TxState newState);
};

} // namespace physicallayer
} // namespace inet

#endif // ifndef __INET_ETHERPHY_H

