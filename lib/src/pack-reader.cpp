#include "file-table.hpp"

#include "zconf.h"
#include <fstream>

#include <ios>
#include <iostream>
#include <pack-reader.hpp>
#include <vector>
#include <zlib.h>
namespace AssetPacker
{

PackReader::PackReader (const std::string& path) : pathToPackedFile (path)
{
    std::ifstream ifstream (pathToPackedFile, std::ios::binary);

    table.readTableHeader (ifstream);
}

bool PackReader::unpackFile (const std::string& fileName, std::vector<char>& outputBuffer)
{
    if (! table.files.contains (fileName))
    {
        return false;
    }

    auto file = table.files[fileName];

    std::ifstream ifstream (pathToPackedFile, std::ios::binary);

    if (! ifstream.is_open())
    {
        return false;
    }

    ifstream.seekg (file.positionInTable);
    std::vector<char> compressedBuffer (file.compressedSize);
    ifstream.read (compressedBuffer.data(), file.compressedSize);

    std::vector<char> uncompressedBuffer (file.size);
    size_t dstLength = file.size;

    int result = uncompress ((Bytef*) uncompressedBuffer.data(), &dstLength, (Bytef*) compressedBuffer.data(), file.compressedSize);
    if (result != 0)
    {
        return false;
    }

    outputBuffer = std::move (uncompressedBuffer);
    return true;
}

} // namespace AssetPacker