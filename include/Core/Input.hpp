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
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static bool IsKeyPressed(Key key);
        static bool IsJustPressed(Key key);
        static bool IsJustReleased(Key key);

        static bool IsMouseButtonPressed(MouseCode button);
        static bool IsMouseButtonJustPressed(MouseCode button);
        static bool IsMouseButtonJustReleased(MouseCode button);

        static std::pair<double, double> GetMousePosition();
        static std::pair<double, double> GetMouseDelta();
        static std::pair<double, double> GetScrollDelta();

    private:
        inline static std::unordered_map<int, bool> s_keys;
        inline static std::unordered_map<int, bool> s_justPressed;
        inline static std::unordered_map<int, bool> s_justReleased;

        inline static std::unordered_map<int, bool> s_mouseButtons;
        inline static std::unordered_map<int, bool> s_mouseJustPressed;
        inline static std::unordered_map<int, bool> s_mouseJustReleased;

        inline static std::pair<double, double> s_mousePosition = { 0.0, 0.0 };
        inline static std::pair<double, double> s_lastMousePosition = { 0.0, 0.0 };
        inline static std::pair<double, double> s_mouseDelta = { 0.0, 0.0 };

        inline static std::pair<double, double> s_scrollDelta = { 0.0, 0.0 };
        inline static std::pair<double, double> s_lastScrollDelta = { 0.0, 0.0 };
};

#endif