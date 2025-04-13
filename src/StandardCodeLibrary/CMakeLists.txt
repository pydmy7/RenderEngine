set(target StandardCodeLibrary)

add_library(${target} INTERFACE)

file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
target_sources(${target} INTERFACE ${headers})

target_include_directories(${target} INTERFACE include)

set_target_properties(${target} PROPERTIES FOLDER "Libraries")
