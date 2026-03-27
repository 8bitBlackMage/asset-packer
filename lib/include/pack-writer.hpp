#pragma once

#include "zlib.h"
#include <file-table.hpp>
#include <fstream>
#include <ostream>
#include <progress-function.hpp>
#include <string>
#include <vector>
namespace AssetPacker
{
class PackWriter
{
public:
    PackWriter (const std::string& path);

    void addFileToList (const std::string& filepath);

    template <ProgressFunction F>
    void writePakFile (F&& progressFunction)
    {
        std::ofstream ofstream (pathToPackedFile, std::ios::binary);

        size_t filePosition = table.getTableHeaderSize() + 1;
        ofstream.seekp (filePosition);
        for (size_t i = 0; i < filePaths.size(); i++)
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
            if (result != 0)
            {
                continue; //come back for better error handling here.
            }
            ofstream.write (compressedFile.data(), compressedLength);

            auto fileName = std::filesystem::path (filePath).filename();
            table.files[fileName].positionInTable = filePosition;
            table.files[fileName].size = fileContents.size();
            table.files[fileName].compressedSize = compressedLength;

            filePosition += compressedLength;

            progressFunction (filePaths.size(), fileName);
        }

        table.writeTableHeader (ofstream);
    }

    void writeHeader (std::ostream& stream);

private:
    std::vector<std::string> filePaths;
    FileTable table;
    std::string pathToPackedFile;
};

} // namespace AssetPacker