#include "Gui/Rectangle.hpp"
#include "ECS/Entity.hpp"
#include "Utils/Debug.hpp"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

static const float s_vertices[] = {
    // Triangle 1
    -0.5f, -0.5f, 0.0f, // Bas-gauche
     0.5f, -0.5f, 0.0f, // Bas-droite
     0.5f,  0.5f, 0.0f, // Haut-droite
    // Triangle 2
     0.5f,  0.5f, 0.0f, // Haut-droite
    -0.5f,  0.5f, 0.0f, // Haut-gauche
    -0.5f, -0.5f, 0.0f  // Bas-gauche
};

Rectangle::Rectangle(const Color color, float width, float height) : m_color(color), m_width(width), m_height(height) {}

Rectangle::~Rectangle() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
    }
}

void Rectangle::SetupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Rectangle::Start() {
    SetupMesh();
}

std::string Rectangle::ShaderType() {
    return "shape";
}

void Rectangle::Render(Shader& shader) {
    if (m_VAO == 0) {
        Debug::Error("Rectangle::Render: VAO non initialisé. La méthode Start() a-t-elle été appelée ?");
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    shader.Use();

    glm::mat4 model = m_owner->GetTransform().GetModelMatrix();
    model = glm::scale(model, glm::vec3(m_width, m_height, 1.0f));

    shader.SetMat4("model", model);
    shader.SetVec4("shapeColor", glm::vec4(m_color.r, m_color.g, m_color.b, m_color.alpha));

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}