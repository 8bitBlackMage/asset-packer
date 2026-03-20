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
        stream.write (posArray.data(), posArray.size());
        auto sizeArray = to_bytes (file.second.size);
        stream.write (sizeArray.data(), sizeArray.size());
        stream << '\0';
    }
}

void PackWriter::addFileToList (const std::string& filePath)
{
    filePaths.push_back (filePath);
    auto path = std::filesystem::path (filePath);
    table.files[path.filename()] = File { 0, 0, 0 };
}

void PackWriter::writePakFile()
{
    std::ofstream ofstream (pathToPackedFile, std::ios::binary);

    size_t filePosition = table.getTableHeaderSize() + 1;
    ofstream.seekp (filePosition);
    for (int i = 0; i < filePaths.size(); i++)
    {
        auto filePath = filePaths[i];

        std::ifstream file (filePath, std::ios::binary | std::ios::ate);
        size_t fileSize = file.tellg();

        file.seekg (std::ios::beg);

        std::vector<char> fileContents (fileSize);
        file.read (fileContents.data(), fileSize);

        size_t compressedLength = compressBound (fileContents.size());
        std::vector<char> compressedFile (compressedLength);

        auto result = compress ((Bytef*) compressedFile.data(), &compressedLength, (Bytef*) fileContents.data(), fileContents.size());

        ofstream.write (compressedFile.data(), compressedLength);

        auto fileName = std::filesystem::path (filePath).filename();
        table.files[fileName].positionInTable = filePosition;
        table.files[fileName].size = fileContents.size();
        table.files[fileName].compressedSize = compressedLength;

        filePosition += compressedLength;
    }

    table.writeTableHeader (ofstream);
}

} // namespace AssetPacker