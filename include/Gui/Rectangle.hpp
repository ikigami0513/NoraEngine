#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <glm/glm.hpp>

#include "Gui/GuiComponent.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Color.hpp"

class Rectangle : public GuiComponent {
    public:
        Rectangle(const Color color = Color(1.0f, 1.0f, 1.0f, 1.0f), float width = 100, float height = 50);

        ~Rectangle();

        void Start() override;
        void Render(Shader& shader) override;
        std::string ShaderType();

        void SetColor(const Color color) {
            m_color = color;
        }

        Color GetColor() const {
            return m_color;
        }

        void SetWidth(float width) {
            m_width = width;
        }

        float GetWidth() {
            return m_width;
        }

        void SetHeight(float height) {
            m_height = height;
        }

        float GetHeight() {
            return m_height;
        }

    private:
        void SetupMesh();

        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;

        Color m_color;

        float m_width;
        float m_height;
};

#endif