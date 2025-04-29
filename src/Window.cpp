#include "Window.hpp"
#include "Time.hpp"

Window& Window::GetInstance() {
    static Window instance;
    return instance;
}

Window::Window() {
    try {
        py::module_ sys = py::module_::import("sys");
        sys.attr("path").cast<py::list>().insert(0, py::str("../"));
        m_game = std::make_unique<py::module_>(py::module_::import("game"));
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
    glfwSetWindowUserPointer(m_window, this);
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
        glClearColor(BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, BackgroundColor.alpha);
        glClear(GL_COLOR_BUFFER_BIT);

        m_game->attr("update")();
    }
    catch (const py::error_already_set& e) {
        std::cerr << "Python error during render: " << e.what() << std::endl;
    }
}

void Window::Shutdown() {
    m_game = std::make_unique<py::object>(); // Reset to null object
}

void Window::Run() {
    InitGLFW();
    CreateWindow();
    InitGLAD();

    py::object initialize = m_game->attr("initialize")();

    while (!glfwWindowShouldClose(m_window)) {
        float currentTime = glfwGetTime();
        Time::UpdateDeltaTime(currentTime);
        Time::CalculateFPS(currentTime);
        ProcessInput();
        Render();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    Shutdown();
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->OnResize(width, height);
    }
}

void Window::OnResize(int width, int height) {
    glViewport(0, 0, width, height);
    m_width = width;
    m_height = height;
}

std::string Window::GetTitle() const {
    return m_title;
}

void Window::SetTitle(const std::string& title) {
    if (m_window) {
        glfwSetWindowTitle(m_window, title.c_str());
        m_title = title;
    }
}

std::pair<int, int> Window::GetSize() const {
    return {m_width, m_height};
}

void Window::SetSize(int width, int height) {
    m_width = width;
    m_height = height;
    if (m_window) {
        glfwSetWindowSize(m_window, m_width, m_height);
    }
}
