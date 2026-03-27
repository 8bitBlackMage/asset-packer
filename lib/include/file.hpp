#pragma once

#include <cstddef>
#include <vector>

namespace AssetPacker
{
class File
{
public:
    File();
    File (std::vector<char>& data);

    constexpr size_t getSize() const noexcept
    {
        return fileData.size();
    }
    constexpr void* getData() noexcept
    {
        return fileData.data();
    }

private:
    std::vector<char> fileData;
};
} // namespace AssetPacker