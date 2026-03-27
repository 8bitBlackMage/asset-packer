cmake_minimum_required(VERSION 3.25)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



project(lib-asset-packer)

add_library(${PROJECT_NAME} STATIC)

find_package(ZLIB REQUIRED)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_sources(${PROJECT_NAME} PUBLIC 
                src/pack-reader.cpp
                src/pack-writer.cpp      
                src/file-table.cpp
                src/file.cpp
                )

target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)