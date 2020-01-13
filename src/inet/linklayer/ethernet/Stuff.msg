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

import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;

namespace inet;

class Ieee8023MacAddresses extends FieldsChunk
{
    chunkLength = B(12);
}

class Ieee8023TypeOrLength extends FieldsChunk
{
    chunkLength = B(2);
}

//class Ieee8023MacHeader extends FieldsChunk // Ieee8023MacAddresses, Ieee8023TypeOrLength
//{
//    chunkLength = B(14);
//}

class Ieee8021QHeader extends Ieee8023TypeOrLength
{
    chunkLength = B(4);
}

class Ieee8021AeHeader extends Ieee8023TypeOrLength
{
    // TODO: chunkLength = B(2+);
}
