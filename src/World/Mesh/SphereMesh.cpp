#include "World/Mesh/SphereMesh.hpp"
#include "World/Entity.hpp"
#include <cmath>
#include <iostream>

SphereMesh::SphereMesh(unsigned int sectorCount, unsigned int stackCount)
    : m_sectorCount(sectorCount), m_stackCount(stackCount) {}

SphereMesh::~SphereMesh() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
    }
}

void SphereMesh::GenerateVertices() {
    m_vertices.clear();
    const float PI = 3.14159265359f;

    // Génération des sommets et des coordonnées de texture
    for (unsigned int i = 0; i <= m_stackCount; ++i) {
        float stackAngle = PI / 2.0f - i * PI / m_stackCount; // de pi/2 à -pi/2
        float xy = cosf(stackAngle); // rayon du cercle
        float z = sinf(stackAngle);  // hauteur

        for (unsigned int j = 0; j <= m_sectorCount; ++j) {
            float sectorAngle = j * 2.0f * PI / m_sectorCount; // de 0 à 2pi

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            float u = static_cast<float>(j) / m_sectorCount;
            float v = static_cast<float>(i) / m_stackCount;

            // position (x, y, z), texture coords (u, v)
            m_vertices.push_back(x);
            m_vertices.push_back(y);
            m_vertices.push_back(z);
            m_vertices.push_back(u);
            m_vertices.push_back(v);
        }
    }

    // Construction des triangles (sans indices)
    std::vector<float> expanded;
    for (unsigned int i = 0; i < m_stackCount; ++i) {
        for (unsigned int j = 0; j < m_sectorCount; ++j) {
            unsigned int first = i * (m_sectorCount + 1) + j;
            unsigned int second = first + m_sectorCount + 1;

            // Triangle 1 : sommets first, second, (first + 1)
            for (int k = 0; k < 5; ++k)
                expanded.push_back(m_vertices[first * 5 + k]);
            for (int k = 0; k < 5; ++k)
                expanded.push_back(m_vertices[second * 5 + k]);
            for (int k = 0; k < 5; ++k)
                expanded.push_back(m_vertices[(first + 1) * 5 + k]);

            // Triangle 2 : sommets (first + 1), second, (second + 1)
            for (int k = 0; k < 5; ++k)
                expanded.push_back(m_vertices[(first + 1) * 5 + k]);
            for (int k = 0; k < 5; ++k)
                expanded.push_back(m_vertices[second * 5 + k]);
            for (int k = 0; k < 5; ++k)
                expanded.push_back(m_vertices[(second + 1) * 5 + k]);
        }
    }

    m_vertices = std::move(expanded);
}


void SphereMesh::SetupMesh() {
    GenerateVertices();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SphereMesh::Start() {
    SetupMesh();
}

void SphereMesh::Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    shader.Use();
    m_texture->Bind(0);

    shader.SetMat4("projection", projectionMatrix);
    shader.SetMat4("view", viewMatrix);

    glm::mat4 modelMatrix = m_owner->GetTransform().GetModelMatrix();
    shader.SetMat4("model", modelMatrix);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size() / 5));
    glBindVertexArray(0);

    m_texture->Unbind(0);
}

std::string SphereMesh::ShaderType() {
    return "mesh";
}
