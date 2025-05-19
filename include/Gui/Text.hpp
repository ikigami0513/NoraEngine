#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <memory>
#include "Gui/Font.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Color.hpp"
#include "Gui/GuiComponent.hpp"

class Text: public GuiComponent {
    public:
        Text();
        ~Text();

        void Start() override;
        void Render(Shader& shader) override;

        std::unique_ptr<Font> font;
        std::string text;
        Color color;

    private:
        void SetupMesh();
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
};

#endif