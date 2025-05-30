#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <memory>

#include "World/Component.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

class Sprite : public Component {
    public:
        Sprite();
        ~Sprite();

        void Start() override;
        void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
        std::string ShaderType();

        void SetTexture(std::shared_ptr<Texture> texture) {
            m_texture = std::move(texture);
        }

        Texture GetTexture() {
            return *m_texture;
        }

    private:
        void SetupMesh();
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;

        static const float s_vertices[30];
        std::shared_ptr<Texture> m_texture;
};

#endif