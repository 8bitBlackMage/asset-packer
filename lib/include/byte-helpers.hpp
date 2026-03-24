#pragma once
#include <algorithm>
#include <array>

template <typename T>
std::array<unsigned char, sizeof (T)> to_bytes (const T& object)
{
    std::array<unsigned char, sizeof (T)> bytes;

    const unsigned char* begin = reinterpret_cast<const unsigned char*> (std::addressof (object));
    const unsigned char* end = begin + sizeof (T);
    std::copy (begin, end, std::begin (bytes));

    return bytes;
}

template <typename T>
T& from_bytes (const std::array<unsigned char, sizeof (T)>& bytes, T& object)
{
    static_assert (std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

    unsigned char* begin_object = reinterpret_cast<unsigned char*> (std::addressof (object));
    std::copy (std::begin (bytes), std::end (bytes), begin_object);

    return object;
}