#include "asset-packer.hpp"
#include "zconf.h"
#include <byte-helpers.hpp>
#include <filesystem>
#include <fstream>
#include <ios>
#include <ostream>
#include <pack-writer.hpp>
#include <vector>
#include <zlib.h>
const std::string headerTest = "FILES";

namespace AssetPacker
{
PackWriter::PackWriter (const std::string& path) : pathToPackedFile (path)
{
}

void PackWriter::writeHeader (std::ostream& stream)
{
    stream.seekp (std::ios::beg);
    stream << AssetPacker::magic;

    //fill in table.
    for (auto& file : table.files)
    {
        stream << file.first << '\0';
        auto posArray = to_bytes (file.second.positionInTable);
        stream.write ((char*) posArray.data(), posArray.size());
        auto sizeArray = to_bytes (file.second.size);
        stream.write ((char*) sizeArray.data(), sizeArray.size());
        stream << '\0';
    }
}

void PackWriter::addFileToList (const std::string& filePath)
{
    filePaths.push_back (filePath);
    auto path = std::filesystem::path (filePath);
    table.files[path.filename()] = File { 0, 0, 0 };
}

} // namespace AssetPacker