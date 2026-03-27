#pragma once
#include <file.hpp>
#include <map>
#include <ostream>

namespace AssetPacker
{
/**
 * @brief Class that represents the table of files within a .PAK file. 
 * 
*/
class FileTable
{
public:
    /**
     * @brief 
     * 
    */
    struct FileTableEntry
    {
        /**
    * @brief Size of the file in bytes. 
    */
        size_t size;
        /**
     * @brief Size of the compressed binary data within the PAk file.

    */
        size_t compressedSize;
        /**
     * @brief Position of the file in table. 
    */
        size_t positionInTable;
    };

    /**
     * @brief Writes the file table to a given filestream.
     * 
     * @param filestream filestream to write to.
    */
    void writeTableHeader (std::ostream& filestream);

    /**
     * @brief Get the Table Header Size.
     * 
     * @return size_t size of the table header in bytes.
    */
    size_t getTableHeaderSize() const;

    /**
     * @brief Reset the state of the file table.
     * 
    */
    inline void reset() { files.clear(); }

    /**
     * @brief Read from the file stream to populate the header data.
     * 
     * @param filestream filestream to read from.
    */
    void readTableHeader (std::istream& filestream);

    std::map<std::string, FileTableEntry> files;

    static constexpr std::array<char, 3> magic = { 'P', 'A', 'K' };
    static constexpr unsigned int versionNumber = 1;

private:
    size_t getTableSize() const;
};

} // namespace AssetPacker