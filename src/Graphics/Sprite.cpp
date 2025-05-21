#include "Graphics/Sprite.hpp"
#include "World/Entity.hpp"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

const float Sprite::s_vertices[30] = {
    // Position (x, y, z)    // Texture coords (u, v)
    -0.5f, -0.5f, 0.0f,      0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,      1.0f, 0.0f,
     0.5f,  0.5f, 0.0f,      1.0f, 1.0f,

     0.5f,  0.5f, 0.0f,      1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,      0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,      0.0f, 0.0f
};

Sprite::Sprite() {}

Sprite::~Sprite() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
    }
}

void Sprite::SetupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sprite::Start() {
    SetupMesh();
}

void Sprite::Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    shader.Use();
    if (m_texture) {
        m_texture->Bind(0);
    }

    shader.SetMat4("projection", projectionMatrix);
    shader.SetMat4("view", viewMatrix);
    glm::mat4 modelMatrix = m_owner->GetTransform().GetModelMatrix();
    shader.SetMat4("model", modelMatrix);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    if (m_texture) {
        m_texture->Unbind(0);
    }
}

std::string Sprite::ShaderType() {
    return "mesh";
}
