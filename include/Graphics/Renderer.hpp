#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "ECS/Scene.hpp"

class Renderer {
    public:
        bool Rendering3D(Scene scene, int width, int height);
        bool Rendering2D(Scene scene, int width, int height);
        void RenderingGUI(Scene scene, int width, int height);
};

#endif