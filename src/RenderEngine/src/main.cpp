#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <implot.h>
#include <implot_internal.h>

#include <iostream>
#include <format>
#include <thread>

namespace glfw {

GLFWwindow* init() {
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << std::format("GLFW Error {}: {}", error, description) << std::endl;
    });

    if (!glfwInit()) {
        std::exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "RenderEngine", nullptr, nullptr);
    if (window == nullptr) {
        std::exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, []([[maybe_unused]] GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });
    glfwSwapInterval(1);

    return window;
}

}  // namespace glfw

namespace opengl {

void init() {
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(-1);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback([](
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        [[maybe_unused]] GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar* message,
        [[maybe_unused]] const void* userParam
    ) {
        std::cerr << "OpenGL Debug: " << message << std::endl;
    }, nullptr);
}

}  // namespace opengl

namespace imgui {

void init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    if (!ImGui_ImplOpenGL3_Init("#version 460")) {
        std::cerr << "Failed to initialize ImGui OpenGL3" << std::endl;
        std::exit(-1);
    }
}

void renderLoopBegin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void renderLoopEnd() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void fps() {
    ImGui::Begin("Performance");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}

void colorBar() {
    ImGui::Begin("Color Bar");
    ImPlot::PushColormap(ImPlotColormap_Jet);
    ImPlot::ColormapScale("Color Scale", 0.0f, 1.0f, ImVec2(100, 200));
    ImPlot::PopColormap();
    ImGui::End();
}

}  // namespace imgui

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    auto window = glfw::init();
    opengl::init();
    imgui::init(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard && !io.WantCaptureMouse) {
            processInput(window);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        imgui::renderLoopBegin();

        imgui::fps();
        imgui::colorBar();

        imgui::renderLoopEnd();

        glfwSwapBuffers(window);
    }

    imgui::destroy();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
