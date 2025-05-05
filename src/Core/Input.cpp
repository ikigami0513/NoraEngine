#include "Core/Input.hpp"

void Input::Init() {
    s_keys.clear();
}

void Input::EndFrame() {
    s_justPressed.clear();
    s_justReleased.clear();
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        s_justPressed[key] = s_keys[key] == false;
        s_keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        s_justPressed[key] = false;
        s_justReleased[key] = true;
        s_keys[key] = false;
    }
}

bool Input::IsKeyPressed(Key key) {
    int key_ = static_cast<int>(key);
    auto it = s_keys.find(key_);
    return it != s_keys.end() && it->second;
}

bool Input::IsJustPressed(Key key) {
    int key_ = static_cast<int>(key);
    auto it = s_justPressed.find(key_);
    return it != s_justPressed.end() && it->second;
}

bool Input::IsJustReleased(Key key) {
    int key_ = static_cast<int>(key);
    auto it = s_justReleased.find(key_);
    return it != s_justReleased.end() && it->second;
}
