/*
LodePNG Utils

Copyright (c) 2005-2012 Lode Vandevenne

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#include <string>
#include <vector>

#pragma once

namespace lodepng
{

/*
Get the names and sizes of all chunks in the PNG file.
Returns 0 if ok, non-0 if error happened.
*/
unsigned getChunkInfo(std::vector<std::string>& names, std::vector<size_t>& sizes,
                      const std::vector<unsigned char>& buffer);

/*
Get the filtertypes of each scanline in this PNG file.
Returns 0 if ok, non-0 if error happened.
*/
unsigned getFilterTypes(std::vector<unsigned char>& filterTypes, const std::vector<unsigned char>& buffer);



struct ZlibBlockInfo
{
  int btype; //block type (0-2)
  size_t compressedbits; //size of compressed block in bits
  size_t uncompressedbytes; //size of uncompressed block in bytes

  // only filled in for block type 2
  size_t treebits; //encoded tree size in bits
  std::vector<int> clcl; //19 code length code lengths (compressed tree's tree)
  std::vector<int> treecodes; //N tree codes, with values 0-18. Values 17 or 18 are followed by the repetition value.

  // only filled in for block type 2
  std::vector<int> litlenlengths; //288 code lengths for lit/len symbols
  std::vector<int> distlengths; //32 code lengths for dist symbols

  // only filled in for block types 1 or 2
  std::vector<int> lz77; //LZ77 codes. 0-255: literals. 256: end symbol. 257-285: length/distance pairs: followed by two numbers: length and distance.
  size_t numlit; //number of lit codes in this block
  size_t numlen; //number of len codes in this block
};

//Extracts all info needed from a PNG file to reconstruct the zlib compression exactly.
void extractZlibInfo(std::vector<ZlibBlockInfo>& zlibinfo, const std::vector<unsigned char>& in);

} // namespace lodepng
