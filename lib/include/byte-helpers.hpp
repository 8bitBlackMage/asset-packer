#pragma once
#include <algorithm>
#include <array>

template <typename T>
std::array<char, sizeof (T)> to_bytes (const T& object)
{
    std::array<char, sizeof (T)> bytes;

    const char* begin = reinterpret_cast<const char*> (std::addressof (object));
    const char* end = begin + sizeof (T);
    std::copy (begin, end, std::begin (bytes));

    return bytes;
}

template <typename T>
T& from_bytes (const std::array<char, sizeof (T)>& bytes, T& object)
{
    static_assert (std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

    char* begin_object = reinterpret_cast<char*> (std::addressof (object));
    std::copy (std::begin (bytes), std::end (bytes), begin_object);

    return object;
}