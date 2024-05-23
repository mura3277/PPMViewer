cmake_minimum_required(VERSION 3.22)

project(PPMViewer LANGUAGES CXX)
set(target "PPMViewer")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory("${CMAKE_SOURCE_DIR}/../raylib/raylib" raylib EXCLUDE_FROM_ALL)
add_subdirectory("${CMAKE_SOURCE_DIR}/../imgui" imgui EXCLUDE_FROM_ALL)
add_subdirectory("${CMAKE_SOURCE_DIR}/../rlImGui" rlImGui EXCLUDE_FROM_ALL)
add_subdirectory("${CMAKE_SOURCE_DIR}/../tinyfiledialogs" TFD EXCLUDE_FROM_ALL)

add_executable(${target} MACOSX_BUNDLE main.cxx ppm.cxx)
target_link_libraries(${target} PUBLIC raylib imgui rlImGui TFD)