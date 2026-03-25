#include <byte-helpers.hpp>
#include <filesystem>

#include <pack-writer.hpp>
#include <zlib.h>

namespace AssetPacker
{
PackWriter::PackWriter (const std::string& path) : pathToPackedFile (path)
{
}

void PackWriter::addFileToList (const std::string& filePath)
{
    filePaths.push_back (filePath);
    auto path = std::filesystem::path (filePath);
    table.files[path.filename()] = File { 0, 0, 0 };
}

} // namespace AssetPacker