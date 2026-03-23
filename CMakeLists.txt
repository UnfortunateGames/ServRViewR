cmake_minimum_required(VERSION 3.22)

project(ServRViewR VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXTENSIONS OFF)

add_executable(serviewr)
add_library(servlib)

target_sources(
    serviewr
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
)

target_sources(
    servlib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cmd/commands.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/input/inpututil.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/parsers.cc
)

target_include_directories(
    servlib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cmd
)

target_link_libraries(
    serviewr
    PRIVATE
        servlib
)
