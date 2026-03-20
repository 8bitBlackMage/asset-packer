cmake_minimum_required(VERSION 3.25)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(assetpacker)

add_executable(${PROJECT_NAME}) 

add_subdirectory(vendor/CLI11)
add_subdirectory(vendor/indicators)
target_sources(${PROJECT_NAME}
    PUBLIC
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE lib-asset-packer CLI11 indicators)