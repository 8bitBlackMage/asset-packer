#pragma once

#include <cstddef>
struct File
{
    size_t size;
    size_t compressedSize;
    size_t positionInTable;
};