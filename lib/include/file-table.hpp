#pragma once
#include <asset-packer.hpp>
#include <map>

namespace AssetPacker
{

class FileTable
{
public:
    //writing functions.
    void writeTableHeader (std::ostream& filestream);
    size_t getTableHeaderSize();

    void readTableHeader (std::istream& filestream);

    std::map<std::string, File> files;

private:
    size_t getTableSize();
};

} // namespace AssetPacker