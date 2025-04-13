set(target RenderEngine)
add_executable(${target})
deploy(${target})

target_link_libraries(${target} PRIVATE
    spdlog::spdlog EnTT::EnTT
)
