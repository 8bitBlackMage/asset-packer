
#include <array>
#include <byte-helpers.hpp>
#include <cassert>
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
    filestream << magic.data();

    auto versionArray = to_bytes (versionNumber);

    filestream.write ((char*) versionArray.data(), versionArray.size());

    auto tableSize = to_bytes (getTableSize());
    filestream.write ((char*) tableSize.data(), tableSize.size());
    //fill in table.
    for (const auto& file : files)
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

size_t FileTable::getTableHeaderSize() const
{
    size_t ret = 0;
    ret += magic.size() + 1;
    ret += sizeof (int);
    ret += sizeof (size_t); //table size
    ret += getTableSize();
    return ret;
}

size_t FileTable::getTableSize() const
{
    size_t ret = 0;
    for (const auto& entry : files)
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
    std::array<char, 3> magicCheck;
    filestream.read (magicCheck.data(), sizeof (magicCheck));

    if (magicCheck != magic)
    {
        return;
    }

    unsigned int fileVersion = 0;

    std::array<unsigned char, sizeof (int)> versionArray;

    filestream.read ((char*) versionArray.data(), sizeof (int));
    from_bytes (versionArray, fileVersion);

    assert (versionNumber == fileVersion);

    std::array<unsigned char, sizeof (size_t)> tableSizeBuff;
    filestream.read ((char*) tableSizeBuff.data(), tableSizeBuff.size());

    size_t tableSize;
    from_bytes (tableSizeBuff, tableSize);
    size_t positionCounter = 0;
    while (positionCounter < tableSize)
    {
        std::string fileName;
        std::getline (filestream, fileName, '\0');

        size_t filePosition;
        std::array<unsigned char, sizeof (size_t)> filePositionBuff;
        filestream.read ((char*) filePositionBuff.data(), filePositionBuff.size());
        from_bytes (filePositionBuff, filePosition);

        size_t fileSize;
        std::array<unsigned char, sizeof (size_t)> fileSizeBuff;
        filestream.read ((char*) fileSizeBuff.data(), fileSizeBuff.size());
        from_bytes (fileSizeBuff, fileSize);

        size_t compressedFileSize;
        std::array<unsigned char, sizeof (size_t)> compressedFileSizeBuff;
        filestream.read ((char*) compressedFileSizeBuff.data(), compressedFileSizeBuff.size());
        from_bytes (compressedFileSizeBuff, compressedFileSize);

        files[fileName] = FileTableEntry { fileSize, compressedFileSize, filePosition };
        positionCounter += (fileName.length() + 1) + sizeof (size_t) + sizeof (size_t) + sizeof (size_t);
    }
}
} // namespace AssetPacker