cmake_minimum_required(VERSION 3.25)
include(FetchContent)


project(assetpacker)

add_executable(${PROJECT_NAME}) 

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.6.2
)

FetchContent_Declare(
    indicators
    GIT_REPOSITORY https://github.com/p-ranav/indicators.git
)

FetchContent_MakeAvailable(CLI11 indicators)


target_sources(${PROJECT_NAME}
    PUBLIC
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE lib-asset-packer CLI11 indicators)