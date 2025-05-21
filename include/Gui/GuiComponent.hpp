#ifndef GUI_COMPONENT_HPP
#define GUI_COMPONENT_HPP

#include "World/Component.hpp"
#include "Graphics/Shader.hpp"
#include <glad/glad.h>
#include <memory>

class GuiComponent : public Component {
    public:
        virtual ~GuiComponent() = default;

        virtual void Start() override = 0;
        virtual void Render(Shader& shader) = 0;
        std::string ShaderType() {
            return "gui";
        }
};

#endif