#include "Core/Window.hpp"
#include "Core/Time.hpp"
#include "World/Camera.hpp"
#include "World/Entity.hpp"
#include "World/Mesh/CuboidMesh.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
    glfwSetCursorPosCallback(m_window, CursorPosCallback);
    glfwSetScrollCallback(m_window, ScrollCallback);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::InitGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Window::Setup() {
    m_shader = std::make_unique<Shader>("../resources/shaders/vert.glsl", "../resources/shaders/frag.glsl");
}

void Window::ProcessInput() {
    if (Input::IsKeyPressed(Key::Escape)) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void Window::Update() {
    m_scene.Update();
}

void Window::Render() {
    glClearColor(BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, BackgroundColor.alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<Entity*> cameraEntities = m_scene.GetEntitiesWithComponent<Camera>();
    if (!cameraEntities.empty()) {
        Camera* camera = cameraEntities[0]->GetComponent<Camera>();
        
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), (float)m_width / (float)m_height, 0.1f, 100.0f);

        // camera/view transformation
        glm::mat4 view = camera->GetViewMatrix();

        // render boxes
        std::vector<Entity*> cuboidEntities = m_scene.GetEntitiesWithComponent<CuboidMesh>();
        for (auto entity : cuboidEntities) {
            entity->GetComponent<CuboidMesh>()->Render(*m_shader, view, projection);
        }
    }
}

void Window::Shutdown() {
    m_game = std::make_unique<py::object>(); // Reset to null object
}

void Window::Run() {
    InitGLFW();
    CreateWindow();
    InitGLAD();
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
    Setup();

    try {
        py::object initialize = m_game->attr("initialize")();
    }
    catch (const py::error_already_set& e) {
        std::cerr << "Python error during initialize: " << e.what() << std::endl;
    }

    m_scene.Start();
    
    while (!glfwWindowShouldClose(m_window)) {
        float currentTime = glfwGetTime();
        Time::UpdateDeltaTime(currentTime);
        Time::CalculateFPS(currentTime);

        ProcessInput();
        Update();
        Render();

        Input::EndFrame();
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

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Input::KeyCallback(window, key, scancode, action, mods);
}

void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Input::MouseButtonCallback(window, button, action, mods);
}

void Window::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Input::CursorPosCallback(window, xpos, ypos);
}

void Window::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Input::ScrollCallback(window, xoffset, yoffset);
}

std::unique_ptr<py::object> Window::Game() {
    return std::move(m_game);
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

Scene& Window::GetScene() {
    return m_scene;
}

void Window::SetScene(const Scene& scene) {
    m_scene = scene;
}
