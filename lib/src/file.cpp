#include <file.hpp>
#include <vector>

namespace AssetPacker
{
File::File (std::vector<char>& data) : fileData (data)
{
}
} // namespace AssetPacker