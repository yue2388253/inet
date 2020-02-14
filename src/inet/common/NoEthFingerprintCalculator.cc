//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "inet/common/ModuleAccess.h"
#include "inet/common/NoEthFingerprintCalculator.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/ethernet/EtherMac.h"
#include "inet/linklayer/ethernet/EtherMacFullDuplex.h"
#include "inet/physicallayer/ethernet/EtherPhy.h"

namespace inet {

Register_Class(NoEthFingerprintCalculator);

void NoEthFingerprintCalculator::addEvent(cEvent *event)
{
    if (event->isMessage()) {
        auto message = static_cast<cMessage *>(event);
        auto arrivalModule = message->getArrivalModule();
        if (dynamic_cast<EtherMac*>(arrivalModule)
                || dynamic_cast<EtherMacFullDuplex*>(arrivalModule)
                || dynamic_cast<physicallayer::EtherPhy*>(arrivalModule)
                )
            return;
    }
    cSingleFingerprintCalculator::addEvent(event);
}

} // namespace

