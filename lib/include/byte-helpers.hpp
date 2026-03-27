#pragma once
#include <algorithm>
#include <array>
/**
 * @brief Convert a variable to an array of bytes, used for serialization. 
 * 
 * @tparam T Type of object being serialized.
 * @param object Object to be serialized.
 * @return std::array<unsigned char, sizeof (T)>  Array containing serialized data.
*/
template <typename T>
std::array<unsigned char, sizeof (T)> to_bytes (const T& object)
{
    std::array<unsigned char, sizeof (T)> bytes;

    const unsigned char* begin = reinterpret_cast<const unsigned char*> (std::addressof (object));
    const unsigned char* end = begin + sizeof (T);
    std::copy (begin, end, std::begin (bytes));

    return bytes;
}

/**
 * @brief Convert an array of bytes to a variable. used for deserialization. 
 * 
 * @tparam T Type of object being deserialized
 * @param bytes Array of bytes to be converted.
 * @param object Object to be populated.
 * @return T& Deserialized object.
*/
template <typename T>
T& from_bytes (const std::array<unsigned char, sizeof (T)>& bytes, T& object)
{
    static_assert (std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

    unsigned char* begin_object = reinterpret_cast<unsigned char*> (std::addressof (object));
    std::copy (std::begin (bytes), std::end (bytes), begin_object);

    return object;
}