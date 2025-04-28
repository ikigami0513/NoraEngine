#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <pybind11/embed.h>
#include <iostream>
#include <string>

namespace py = pybind11;

class Window {
    public:
        Window();
        ~Window();

        void Run();

    private:
        void InitGLFW();
        void CreateWindow();
        void InitGLAD();
        void ProcessInput();
        void Render();

        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    private:
        GLFWwindow* m_window = nullptr;
        std::string m_title = "Nora Engine";
        int m_width = 800;
        int m_height = 600;

        py::module_ m_nora;
};

#endif