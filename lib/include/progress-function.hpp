#pragma once

#include <string>

template <class T>
concept ProgressFunction = requires (T t)
{
    t (int {}, std::string {});
};