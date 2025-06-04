#include "Core/Mouse.hpp"
#include "Core/Window.hpp"

MouseMode Mouse::m_currentMode = MouseMode::NORMAL;

void Mouse::SetMode(MouseMode mode) {
    if (!Window::GetInstance().GLFWWindow()) return;

    glfwSetInputMode(Window::GetInstance().GLFWWindow(), GLFW_CURSOR, GLFWMode(mode));
    m_currentMode = mode;
}

MouseMode Mouse::GetMode() {
    return m_currentMode;
}

int Mouse::GLFWMode(MouseMode mode) {
    switch (mode) {
        case MouseMode::NORMAL:   return GLFW_CURSOR_NORMAL;
        case MouseMode::HIDDEN:   return GLFW_CURSOR_HIDDEN;
        case MouseMode::DISABLED: return GLFW_CURSOR_DISABLED;
        default:
            // Gérer le cas d'un mode invalide, peut-être lever une exception
            // ou retourner un mode par défaut.
            throw std::invalid_argument("Invalid mouse mode requested.");
    }
}
