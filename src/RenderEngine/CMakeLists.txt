set(target RenderEngine)
add_executable(${target})
deploy(${target})

target_link_libraries(${target} PRIVATE
    StandardCodeLibrary
    spdlog::spdlog Vulkan::Vulkan
    SDL3::SDL3
)
