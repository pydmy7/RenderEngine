set(target RenderEngine)
add_executable(${target})
deploy(${target})

target_link_libraries(${target} PRIVATE
    StandardCodeLibrary
    spdlog::spdlog Vulkan::Vulkan
)
target_link_libraries(${target} PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
