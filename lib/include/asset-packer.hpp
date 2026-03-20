#pragma once

#include <cstddef>
#include <string>

namespace AssetPacker
{
///@brief struct representing index table in packed file.

const std::string magic = "PAK1\0";

struct File
{
    size_t size;
    size_t compressedSize;
    size_t positionInTable;
};

} // namespace AssetPacker