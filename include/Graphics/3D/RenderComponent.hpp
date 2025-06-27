#ifndef RENDER_COMPONENT_HPP
#define RENDER_COMPONENT_HPP

#include "ECS/Component.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

class RenderComponent : public Component {
    public:
        virtual void Start() override = 0;
        virtual void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) = 0;
        virtual std::string ShaderType() = 0;
};

#endif