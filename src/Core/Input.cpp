#include "Core/Input.hpp"
#include <GLFW/glfw3.h>

void Input::Init() {
    // Rien à initialiser pour l'instant, les maps sont initialisées par défaut.
}

void Input::EndFrame() {
    s_justPressed.clear();
    s_justReleased.clear();
    s_mouseJustPressed.clear();
    s_mouseJustReleased.clear();
    s_mouseDelta = { 0.0, 0.0 };
    s_scrollDelta = { 0.0, 0.0 };
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_RELEASE && !s_keys[key]) {
        s_justPressed[key] = true;
    }
    if (action == GLFW_RELEASE && s_keys[key]) {
        s_justReleased[key] = true;
    }
    s_keys[key] = action != GLFW_RELEASE;
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action != GLFW_RELEASE && !s_mouseButtons[button]) {
        s_mouseJustPressed[button] = true;
    }
    if (action == GLFW_RELEASE && s_mouseButtons[button]) {
        s_mouseJustReleased[button] = true;
    }
    s_mouseButtons[button] = action != GLFW_RELEASE;
}

void Input::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    s_mousePosition = { xpos, ypos };
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    s_scrollDelta = { xoffset, yoffset };
}

bool Input::IsKeyPressed(Key key) {
    return s_keys.count(static_cast<int>(key)) && s_keys[static_cast<int>(key)];
}

bool Input::IsJustPressed(Key key) {
    return s_justPressed.count(static_cast<int>(key)) && s_justPressed[static_cast<int>(key)];
}

bool Input::IsJustReleased(Key key) {
    return s_justReleased.count(static_cast<int>(key)) && s_justReleased[static_cast<int>(key)];
}

bool Input::IsMouseButtonPressed(MouseCode button) {
    return s_mouseButtons.count(static_cast<int>(button)) && s_mouseButtons[static_cast<int>(button)];
}

bool Input::IsMouseButtonJustPressed(MouseCode button) {
    return s_mouseJustPressed.count(static_cast<int>(button)) && s_mouseJustPressed[static_cast<int>(button)];
}

bool Input::IsMouseButtonJustReleased(MouseCode button) {
    return s_mouseJustReleased.count(static_cast<int>(button)) && s_mouseJustReleased[static_cast<int>(button)];
}

std::pair<double, double> Input::GetMousePosition() {
    return s_mousePosition;
}

std::pair<double, double> Input::GetMouseDelta() {
    std::pair<double, double> delta = { s_mousePosition.first - s_lastMousePosition.first, s_mousePosition.second - s_lastMousePosition.second };
    s_mouseDelta = delta;
    s_lastMousePosition = s_mousePosition;
    return s_mouseDelta;
}

std::pair<double, double> Input::GetScrollDelta() {
    return s_scrollDelta;
}
