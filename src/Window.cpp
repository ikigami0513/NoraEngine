#include "Window.hpp"
#include "Time.hpp"

Window::Window() {
    try {
        py::module_ sys = py::module_::import("sys");
        sys.attr("path").cast<py::list>().insert(0, py::str("../"));

        m_nora = py::module_::import("nora");

        m_title = m_nora.attr("__title__").cast<std::string>();
        m_width = m_nora.attr("__width__").cast<int>();
        m_height = m_nora.attr("__height__").cast<int>();
    }
    catch (const py::error_already_set& e) {
        std::cerr << "Python error: " << e.what() << std::endl;
    }
}

Window::~Window() {
    glfwTerminate();
}

void Window::InitGLFW() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void Window::CreateWindow() {
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
}

void Window::InitGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Window::ProcessInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void Window::Render() {
    try {
        py::object bg_color = m_nora.attr("background_color");
        glClearColor(
            bg_color.attr("r").cast<float>(),
            bg_color.attr("g").cast<float>(),
            bg_color.attr("b").cast<float>(),
            bg_color.attr("alpha").cast<float>()
        );
        glClear(GL_COLOR_BUFFER_BIT);

        m_nora.attr("update")();
    }
    catch (const py::error_already_set& e) {
        std::cerr << "Python error during render: " << e.what() << std::endl;
    }
}

void Window::Run() {
    InitGLFW();
    CreateWindow();
    InitGLAD();

    while (!glfwWindowShouldClose(m_window)) {
        float currentTime = glfwGetTime();
        Time::UpdateDeltaTime(currentTime);
        Time::CalculateFPS(currentTime);
        ProcessInput();
        Render();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}