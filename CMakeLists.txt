cmake_minimum_required(VERSION 3.16)
project(reversi)

# From SDL wiki
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")


add_subdirectory(deps/SDL EXCLUDE_FROM_ALL)

add_executable(reversi WIN32 src/main.c)
target_link_libraries(reversi PRIVATE SDL3::SDL3)
