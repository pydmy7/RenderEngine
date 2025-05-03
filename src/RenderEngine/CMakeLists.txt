set(target RenderEngine)
add_executable(${target})
deploy(${target})

target_link_libraries(${target} PRIVATE
    StandardCodeLibrary
    spdlog::spdlog EnTT::EnTT
    OpenGL::GL glad::glad glfw imgui::imgui
)
