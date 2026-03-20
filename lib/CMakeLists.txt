cmake_minimum_required(VERSION 3.25)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lib-asset-packer)

add_library(${PROJECT_NAME} STATIC)


find_package(ZLIB)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_sources(${PROJECT_NAME} PUBLIC 
                src/asset-packer.cpp
                src/pack-reader.cpp
                src/pack-writer.cpp      
                src/file-table.cpp
                )

target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)