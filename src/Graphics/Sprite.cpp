#include "Graphics/Sprite.hpp"
#include "World/Entity.hpp"
#include "Core/Utils.hpp"
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
        GL_CHECK_ERROR("glDeleteVertexArrays");
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
        GL_CHECK_ERROR("glDeleteBuffers");
    }
}

void Sprite::SetupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    GL_CHECK_ERROR("glGenVertexArrays + glGenBuffers");

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);
    GL_CHECK_ERROR("glBindVertexArray / glBufferData");

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    GL_CHECK_ERROR("Position attribute");

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    GL_CHECK_ERROR("Texture coordinates attribute");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GL_CHECK_ERROR("Unbind VAO/VBO");
}

void Sprite::Start() {
    SetupMesh();
    GL_CHECK_ERROR("SetupMesh");
}

void Sprite::Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    // Save OpenGL state
    GLboolean wasDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean wasBlendEnabled = glIsEnabled(GL_BLEND);
    GLboolean wasCullFaceEnabled = glIsEnabled(GL_CULL_FACE);

    // Prepare for sprite rendering
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_CHECK_ERROR("Sprite render state setup");

    shader.Use();
    GL_CHECK_ERROR("Shader.Use");

    if (m_texture) {
        m_texture->Bind(0);
        shader.SetInt("image", 0);
        GL_CHECK_ERROR("Texture bind + SetInt(image)");
    }
    else {
        std::cout << "No texture" << std::endl;
    }

    shader.SetMat4("model", m_owner->GetTransform().GetModelMatrix());
    shader.SetMat4("view", viewMatrix);
    shader.SetMat4("projection", projectionMatrix);
    shader.SetVec4("spriteColor", glm::vec4(1.0f));
    GL_CHECK_ERROR("Uniform updates");

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    GL_CHECK_ERROR("glDrawArrays");

    if (m_texture) {
        m_texture->Unbind(0);
        GL_CHECK_ERROR("Texture unbind");
    }

    // Restore previous OpenGL state
    if (wasDepthTestEnabled) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    if (!wasBlendEnabled) glDisable(GL_BLEND);
    if (!wasCullFaceEnabled) glDisable(GL_CULL_FACE);
    GL_CHECK_ERROR("Restore OpenGL state");
}

std::string Sprite::ShaderType() {
    return "sprite";
}
