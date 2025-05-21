#ifndef RENDER_COMPONENT_HPP
#define RENDER_COMPONENT_HPP

#include "World/Component.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

class RenderComponent : public Component {
    public:
        virtual ~RenderComponent() = default;

        virtual void Start() override = 0;
        virtual void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) = 0;
        
        void SetTexture(std::shared_ptr<Texture> texture) {
            m_texture = std::move(texture);
        }

        Texture GetTexture() {
            return *m_texture;
        }

        virtual std::string ShaderType() = 0;

    protected:
        std::shared_ptr<Texture> m_texture;
};

#endif