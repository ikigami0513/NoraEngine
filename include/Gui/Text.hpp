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

enum class Alignment {
    Left,
    Center,
    Right
};

class Text: public GuiComponent {
    public:
        Text();
        ~Text();

        void Start() override;
        void Render(Shader& shader) override;
        std::string ShaderType() override;

        glm::vec2 GetTextBounds(float scale = 1.0f);

        std::shared_ptr<Font> font;
        std::string text;
        Color color;
        Alignment alignmenent = Alignment::Center;
        float margin = 0.0f;

    private:
        void SetupMesh();
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
};

#endif