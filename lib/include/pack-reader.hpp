#pragma once
#include "file-table.hpp"
#include <filesystem>
#include <fstream>
#include <progress-function.hpp>
#include <string>

namespace AssetPacker
{
/**
 * @brief 
 * 
*/
class PackReader
{
public:
    PackReader (const std::string& path);

    template <ProgressFunction F>
    void unpackAllFiles (const std::string& outputPath, F&& progressFunction)
    {
        if (! std::filesystem::is_directory (outputPath))
        {
            std::filesystem::create_directory (outputPath);
        }

        for (auto& file : table.files)
        {
            auto output = unpackFile (file.first);

            auto outFile = std::basic_ofstream<char> (outputPath + "/" + file.first, std::ios::binary);
            outFile.write ((char*) output.getData(), output.getSize());

            progressFunction (table.files.size(), file.first);
        }
    }

    File unpackFile (const std::string& fileName);
    size_t getNumFiles() { return table.files.size(); }

private:
    std::string pathToPackedFile;
    FileTable table;
};

} // namespace AssetPacker