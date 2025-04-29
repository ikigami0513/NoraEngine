#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <pybind11/embed.h>
#include <iostream>
#include <string>
#include "Graphics/Color.hpp"
#include "Graphics/Shader.hpp"
#include "Input.hpp"

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
        void Setup();
        void ProcessInput();
        void Update();
        void Render();
        void Shutdown();

        void OnResize(int width, int height);
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        GLFWwindow* m_window = nullptr;
        std::string m_title = "Nora Engine";
        int m_width = 800;
        int m_height = 600;

        std::unique_ptr<py::object> m_game;
        std::unique_ptr<Shader> m_shader;

        float vertices[18] = {
            // positions         // colors
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
             0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
        };

        unsigned int VAO, VBO;
};

#endif