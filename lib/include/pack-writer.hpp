#pragma once

#include <file-table.hpp>
#include <ostream>
#include <string>
#include <vector>
namespace AssetPacker
{
class PackWriter
{
public:
    PackWriter (const std::string& path);

    void addFileToList (const std::string& filepath);

    void writePakFile();

    void writeHeader (std::ostream& stream);

private:
    std::vector<std::string> filePaths;
    FileTable table;
    std::string pathToPackedFile;
};

} // namespace AssetPacker