cmake_minimum_required(VERSION 3.25)



project(assetpacker)

add_executable(${PROJECT_NAME}) 

add_subdirectory(vendor/CLI11)

set(CMAKE_WARN_DEPRECATED FALSE)
add_subdirectory(vendor/indicators)
set(CMAKE_WARN_DEPRECATED TRUE)

target_sources(${PROJECT_NAME}
    PUBLIC
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE lib-asset-packer CLI11 indicators)