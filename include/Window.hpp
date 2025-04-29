#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <pybind11/embed.h>
#include <iostream>
#include <string>
#include "Graphics/Color.hpp"

namespace py = pybind11;

class Window {
    public:
        static Window& GetInstance();
        ~Window();

        void Run();

        std::string GetTitle() const;
        void SetTitle(const std::string& title);

        std::pair<int, int> GetSize() const;
        void SetSize(int width, int height);

        Color BackgroundColor;

    private:
        Window();
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        void InitGLFW();
        void CreateWindow();
        void InitGLAD();
        void ProcessInput();
        void Render();
        void Shutdown();

        void OnResize(int width, int height);
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

        GLFWwindow* m_window = nullptr;
        std::string m_title = "Nora Engine";
        int m_width = 800;
        int m_height = 600;

        std::unique_ptr<py::object> m_game;
};

#endif