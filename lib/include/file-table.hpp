#pragma once
#include <file.hpp>
#include <map>
#include <ostream>

namespace AssetPacker
{

class FileTable
{
public:
    //writing functions.
    void writeTableHeader (std::ostream& filestream);
    size_t getTableHeaderSize();

    void reset() { files.clear(); }

    void readTableHeader (std::istream& filestream);

    std::map<std::string, File> files;

    const std::string magic = "PAK1\0";

private:
    size_t getTableSize();
};

} // namespace AssetPacker