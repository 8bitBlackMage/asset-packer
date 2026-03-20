#pragma once
#include "file-table.hpp"
#include <filesystem>
#include <fstream>
#include <progress-function.hpp>
#include <string>
#include <vector>

namespace AssetPacker
{
class PackReader
{
public:
    PackReader (const std::string& path);

    template <ProgressFunction F>
    void unpackAllFiles (const std::string& outputPath, F&& progressFunction = {})
    {
        if (! std::filesystem::is_directory (outputPath))
        {
            std::filesystem::create_directory (outputPath);
        }

        for (auto& file : table.files)
        {
            std::vector<char> output;
            if (! unpackFile (file.first, output))
            {
                continue;
            }

            auto outFile = std::basic_ofstream<char> (outputPath + "/" + file.first, std::ios::binary);
            outFile.write (output.data(), output.size());

            progressFunction (table.files.size(), file.first);
        }
    }

    bool unpackFile (const std::string& fileName, std::vector<char>& outputBuffer);
    size_t getNumFiles() { return table.files.size(); }

private:
    std::string pathToPackedFile;
    FileTable table;
};

} // namespace AssetPacker