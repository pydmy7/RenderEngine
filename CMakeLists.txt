cmake_minimum_required(VERSION 3.21)

project(RenderEngine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "[INFO] Global use of ccache to accelerate compile")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

find_package(spdlog CONFIG REQUIRED)
find_package(MKL CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)

target_link_libraries(Eigen3::Eigen INTERFACE MKL::MKL)
target_compile_definitions(Eigen3::Eigen INTERFACE EIGEN_USE_MKL_ALL)

add_subdirectory(cmake)

add_subdirectory(src)
