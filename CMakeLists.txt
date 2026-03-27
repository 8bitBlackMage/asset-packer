cmake_minimum_required(VERSION 3.25)
include(FetchContent)
project(asset-packer LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WARNING_LEVEL )

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.13.0
)

FetchContent_MakeAvailable(Catch2)

add_subdirectory(lib)

add_subdirectory(application)
include(CTest)
add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain lib-asset-packer )
include(Catch)
catch_discover_tests(tests)
