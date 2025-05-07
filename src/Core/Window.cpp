#include "Core/Window.hpp"
#include "Core/Time.hpp"
#include "World/Camera.hpp"
#include "World/Entity.hpp"
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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    texture1 = std::make_unique<Texture>("../resources/textures/container.jpg");
    texture2 = std::make_unique<Texture>("../resources/textures/awesomeface.png", true);

    m_shader->Use();
    m_shader->SetInt("texture1", 0);
    m_shader->SetInt("texture2", 1);
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

        // bind textures on corresponding texture units
        texture1->Bind(0);
        texture2->Bind(1);

        // activate shader
        m_shader->Use();
        
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), (float)m_width / (float)m_height, 0.1f, 100.0f);
        m_shader->SetMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera->GetViewMatrix();
        m_shader->SetMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++) {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // maje sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            m_shader->SetMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glBindVertexArray(0);
    }
}

void Window::Shutdown() {
    m_game = std::make_unique<py::object>(); // Reset to null object
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VBO);
}

void Window::Run() {
    InitGLFW();
    CreateWindow();
    InitGLAD();
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
