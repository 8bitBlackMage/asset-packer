cmake_minimum_required(VERSION 3.25)

project(asset-packer LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(lib)

add_subdirectory(application)

add_subdirectory(vendor/Catch2)

add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain lib-asset-packer )

include(CTest)
include(Catch)
catch_discover_tests(tests)