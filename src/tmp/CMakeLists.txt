cmake_minimum_required (VERSION 3.21)

project ("test_dump")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(/Zi)
add_link_options(/DEBUG)

add_executable (${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE
    dbghelp
    wininet
)