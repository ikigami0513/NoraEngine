#ifndef INPUT_HPP
#define INPUT_HPP

#include <GLFW/glfw3.h>
#include <unordered_map>
#include "Core/Key.hpp"

class Input {
    public:
        static void Init();
        static void EndFrame();

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static bool IsKeyPressed(Key key);
        static bool IsJustPressed(Key key);
        static bool IsJustReleased(Key key);

    private:
        inline static std::unordered_map<int, bool> s_keys;
        inline static std::unordered_map<int, bool> s_justPressed;
        inline static std::unordered_map<int, bool> s_justReleased;
};

#endif