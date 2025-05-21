#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <memory>

#include "World/Mesh/RenderComponent.hpp"
#include "Graphics/Shader.hpp"

class Sprite : public RenderComponent {
    public:
        Sprite();
        ~Sprite();

        void Start() override;
        void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
        std::string ShaderType() override;

    private:
        void SetupMesh();
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;

        static const float s_vertices[30];
};

#endif