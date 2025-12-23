cmake_minimum_required(VERSION 3.16)
project(reversi)

include(deps/cmake/FileEmbed.cmake)
FileEmbedSetup()

# From SDL wiki
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(deps/SDL EXCLUDE_FROM_ALL)
add_subdirectory(deps/SDL_ttf EXCLUDE_FROM_ALL)

# Import assets
FileEmbedAdd(assets/Knewave-Regular.ttf)
FileEmbedAdd(assets/restart_icon.png)

add_executable(reversi WIN32 src/main.c src/draw.c src/board.c src/assets.c)
target_include_directories(reversi PRIVATE include)
target_link_libraries(reversi PRIVATE SDL3::SDL3)
target_link_libraries(reversi PRIVATE SDL3_ttf::SDL3_ttf)
target_link_libraries(reversi PRIVATE file_embed)

