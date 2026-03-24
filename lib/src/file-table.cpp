#include "asset-packer.hpp"
#include <array>
#include <byte-helpers.hpp>
#include <cstddef>
#include <file-table.hpp>
#include <iostream>
#include <istream>
#include <string>
namespace AssetPacker
{

void FileTable::writeTableHeader (std::ostream& filestream)
{
    filestream.seekp (std::ios::beg);
    filestream << AssetPacker::magic;

    auto tableSize = to_bytes (getTableSize());
    filestream.write ((char*) tableSize.data(), tableSize.size());
    //fill in table.
    for (auto& file : files)
    {
        filestream.write (file.first.data(), file.first.length());
        filestream << '\0';
        auto posArray = to_bytes (file.second.positionInTable);
        filestream.write ((char*) posArray.data(), posArray.size());
        auto sizeArray = to_bytes (file.second.size);
        filestream.write ((char*) sizeArray.data(), sizeArray.size());
        auto compressedSizeArray = to_bytes (file.second.compressedSize);
        filestream.write ((char*) compressedSizeArray.data(), compressedSizeArray.size());
    }
}

size_t FileTable::getTableHeaderSize()
{
    size_t ret = 0;
    ret += AssetPacker::magic.size() + 1;
    ret += sizeof (size_t); //table size
    ret += getTableSize();
    return ret;
}

size_t FileTable::getTableSize()
{
    size_t ret = 0;
    for (auto entry : files)
    {
        ret += entry.first.length() + 1;
        ret += sizeof (entry.second.positionInTable);
        ret += sizeof (entry.second.size);
        ret += sizeof (entry.second.compressedSize);
    }
    return ret;
}

void FileTable::readTableHeader (std::istream& filestream)
{
    files.clear();
    filestream.seekg (std::ios::beg);
    char* magicCheck = new char[4];
    filestream.read (magicCheck, 4);

    if (magicCheck != AssetPacker::magic)
    {
        delete[] magicCheck;
        return;
    }
    delete[] magicCheck;

    std::array<unsigned char, 8> tableSizeBuff;
    filestream.read ((char*) tableSizeBuff.data(), tableSizeBuff.size());

    size_t tableSize;
    from_bytes (tableSizeBuff, tableSize);
    int positionCounter = 0;
    while (positionCounter < tableSize)
    {
        std::string fileName;
        std::getline (filestream, fileName, '\0');

        size_t filePosition;
        std::array<unsigned char, 8> filePositionBuff;
        filestream.read ((char*) filePositionBuff.data(), filePositionBuff.size());
        from_bytes (filePositionBuff, filePosition);

        size_t fileSize;
        std::array<unsigned char, 8> fileSizeBuff;
        filestream.read ((char*) fileSizeBuff.data(), fileSizeBuff.size());
        from_bytes (fileSizeBuff, fileSize);

        size_t compressedFileSize;
        std::array<unsigned char, 8> compressedFileSizeBuff;
        filestream.read ((char*) compressedFileSizeBuff.data(), compressedFileSizeBuff.size());
        from_bytes (compressedFileSizeBuff, compressedFileSize);

        files[fileName] = File { fileSize, compressedFileSize, filePosition };
        positionCounter += (fileName.length() + 1) + sizeof (size_t) + sizeof (size_t) + sizeof (size_t);
    }
}
} // namespace AssetPacker