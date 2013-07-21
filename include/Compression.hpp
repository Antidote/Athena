// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>

#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP

#include <Types.hpp>

namespace zelda
{
namespace io
{
namespace Compression
{
    Int32 decompressZlib(Uint8* src, Uint32 srcLen, Uint8* dst, Uint32 dstLen);
    void compressZlib(const Uint8* src, Uint32 srcLen, Uint8* dst, Uint32* dstLen);
}
}
}
#endif // COMPRESSION_HPP
