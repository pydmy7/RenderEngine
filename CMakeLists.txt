cmake_minimum_required(VERSION 3.21)

project(LearnSomething)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "[INFO] Global use of ccache to accelerate compile")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

add_subdirectory(cmake)

find_package(vsg CONFIG REQUIRED)
find_package(vsgXchange CONFIG REQUIRED)

add_subdirectory(src)
