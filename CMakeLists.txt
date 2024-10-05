cmake_minimum_required(VERSION 3.15)

project(RenderEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(cmake)

include(3rdlibs/findlibrary.cmake)
add_subdirectory(3rdlibs)

# add_subdirectory(demo)
add_subdirectory(renderengine)
