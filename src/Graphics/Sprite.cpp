#include "Graphics/Sprite.hpp"
#include "World/Entity.hpp"
#include "Core/Utils.hpp"  // Assuming GL_CHECK_ERROR might be here or in Debug
#include "Core/Debug.hpp"  // For Debug::Error, Debug::Warning
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream> // For std::cout in Render, can be removed if Debug handles all messages

// Note: The static const s_vertices array definition is removed from here.
// m_vertices will be initialized in the constructor and updated by SetTextureRect.

void Sprite::InitializeVertices(float u_min, float v_min, float u_max, float v_max) {
    // Positions (quad from -0.5 to 0.5)
    // Vertex 1 (Bottom-Left)
    m_vertices[0] = -0.5f; m_vertices[1] = -0.5f; m_vertices[2] = 0.0f;
    // Vertex 2 (Bottom-Right)
    m_vertices[5] =  0.5f; m_vertices[6] = -0.5f; m_vertices[7] = 0.0f;
    // Vertex 3 (Top-Right)
    m_vertices[10] = 0.5f; m_vertices[11] = 0.5f; m_vertices[12] = 0.0f;

    // Vertex 4 (Top-Right) - for second triangle
    m_vertices[15] = 0.5f; m_vertices[16] = 0.5f; m_vertices[17] = 0.0f;
    // Vertex 5 (Top-Left)
    m_vertices[20] = -0.5f; m_vertices[21] = 0.5f; m_vertices[22] = 0.0f;
    // Vertex 6 (Bottom-Left) - for second triangle
    m_vertices[25] = -0.5f; m_vertices[26] = -0.5f; m_vertices[27] = 0.0f;

    // Texture Coordinates
    // Triangle 1: (BL, BR, TR)
    // BL UVs
    m_vertices[3] = u_min;  m_vertices[4] = v_min;
    // BR UVs
    m_vertices[8] = u_max;  m_vertices[9] = v_min;
    // TR UVs
    m_vertices[13] = u_max; m_vertices[14] = v_max;

    // Triangle 2: (TR, TL, BL)
    // TR UVs
    m_vertices[18] = u_max; m_vertices[19] = v_max;
    // TL UVs
    m_vertices[23] = u_min; m_vertices[24] = v_max;
    // BL UVs
    m_vertices[28] = u_min; m_vertices[29] = v_min;
}

Sprite::Sprite() {
    // Initialize with default texture coordinates (full texture)
    InitializeVertices(0.0f, 0.0f, 1.0f, 1.0f);
    // m_currentTextureRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // If using m_currentTextureRect
}

Sprite::~Sprite() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        // GL_CHECK_ERROR("glDeleteVertexArrays"); // Optional: check error if macro is robust
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
        // GL_CHECK_ERROR("glDeleteBuffers"); // Optional
    }
    // It's good practice to ensure GL_CHECK_ERROR doesn't throw or terminate if called in dtor
    // or to remove them if they cause issues during shutdown.
}

void Sprite::SetupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    GL_CHECK_ERROR("glGenVertexArrays + glGenBuffers");

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // Use m_vertices and GL_DYNAMIC_DRAW since UVs might change
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);
    GL_CHECK_ERROR("glBindVertexArray / glBufferData");

    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    GL_CHECK_ERROR("Position attribute");

    // Texture coordinates attribute (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    GL_CHECK_ERROR("Texture coordinates attribute");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GL_CHECK_ERROR("Unbind VAO/VBO");
}

void Sprite::Start() {
    SetupMesh(); // Now uses m_vertices which might have been set by SetTextureRect before Start
    // SetTextureRect(0.0f, 0.0f, 96.0f, 80.0f);
    GL_CHECK_ERROR("SetupMesh");
}

void Sprite::SetTextureRect(float pixel_x, float pixel_y, float pixel_width, float pixel_height) {
    if (!m_texture) {
        Debug::Warning("Sprite::SetTextureRect - Texture non définie. Impossible de calculer les UVs à partir des pixels.");
        InitializeVertices(0.0f, 0.0f, 1.0f, 1.0f); // Fallback sur la texture entière
        m_currentTextureRectNormalized = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        // Si le VBO existe déjà, il faudrait le mettre à jour avec les vertices par défaut
        if (m_VAO != 0) {
             glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
             glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
             glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        return;
    }

    float texW = static_cast<float>(m_texture->Width());
    float texH = static_cast<float>(m_texture->Height());

    if (texW == 0 || texH == 0) {
        Debug::Warning("Sprite::SetTextureRect - La texture a des dimensions nulles. Impossible de calculer les UVs.");
        InitializeVertices(0.0f, 0.0f, 1.0f, 1.0f); // Fallback
        m_currentTextureRectNormalized = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        if (m_VAO != 0) { /* cf. ci-dessus */ }
        return;
    }

    // Conversion des pixels en UVs normalisés.
    // pixel_x, pixel_y = coin supérieur gauche de la sous-image.
    // UV (0,0) = coin inférieur gauche de la texture.
    float u_min_norm = pixel_x / texW;
    float u_max_norm = (pixel_x + pixel_width) / texW;

    // La coordonnée V doit être inversée car l'axe Y des pixels va vers le bas,
    // alors que l'axe V des UVs va vers le haut.
    // v_max_norm (pour le haut du sprite) correspond au pixel_y (haut de la sous-image)
    float v_max_norm = 1.0f - (pixel_y / texH);
    // v_min_norm (pour le bas du sprite) correspond au pixel_y + pixel_height (bas de la sous-image)
    float v_min_norm = 1.0f - ((pixel_y + pixel_height) / texH);

    // S'assurer que les coordonnées sont dans les bornes [0,1] et que min <= max
    u_min_norm = glm::clamp(u_min_norm, 0.0f, 1.0f);
    u_max_norm = glm::clamp(u_max_norm, 0.0f, 1.0f);
    v_min_norm = glm::clamp(v_min_norm, 0.0f, 1.0f);
    v_max_norm = glm::clamp(v_max_norm, 0.0f, 1.0f);

    if (u_min_norm > u_max_norm) { std::swap(u_min_norm, u_max_norm); }
    if (v_min_norm > v_max_norm) { std::swap(v_min_norm, v_max_norm); }
    
    InitializeVertices(u_min_norm, v_min_norm, u_max_norm, v_max_norm);
    m_currentTextureRectNormalized = glm::vec4(u_min_norm, v_min_norm, u_max_norm, v_max_norm);

    if (m_VAO != 0) { // Si le VAO (et donc VBO) existe déjà, mettre à jour le VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        GL_CHECK_ERROR("Update VBO in SetTextureRect");
    }
}

void Sprite::Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    if (m_texture == nullptr) {
        Debug::Error("Sprite::Render: No Texture linked to Sprite.");
        return;
    }
    if (m_VAO == 0) {
        Debug::Error("Sprite::Render: VAO not initialized. Was Start() called?");
        return;
    }

    // Save OpenGL state
    GLboolean wasDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean wasBlendEnabled = glIsEnabled(GL_BLEND);
    GLboolean wasCullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    GLint oldBlendSrcAlpha, oldBlendDstAlpha, oldBlendEquationAlpha; // For more precise blend state restoration
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &oldBlendSrcAlpha);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &oldBlendDstAlpha);
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &oldBlendEquationAlpha);


    // Prepare for sprite rendering
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE); // Keeps culling enabled, ensure your quad winding is correct
    GL_CHECK_ERROR("Sprite render state setup");

    shader.Use();
    GL_CHECK_ERROR("Shader.Use");

    // Texture binding
    m_texture->Bind(0); // Assuming texture unit 0
    shader.SetInt("image", 0); // Tell shader to use texture unit 0
    GL_CHECK_ERROR("Texture bind + SetInt(image)");
    
    // Model matrix: m_owner is the Entity this component is attached to.
    // GetLocalModelMatrix2D might need adjustment if sprite size should depend on texture rect portion.
    // For now, it scales a unit quad based on the Entity's transform.
    shader.SetMat4("model", m_owner->GetTransform().GetLocalModelMatrix2D(m_texture, m_currentTextureRectNormalized));
    shader.SetMat4("view", viewMatrix);
    shader.SetMat4("projection", projectionMatrix);
    
    // Default sprite color, can be parameterized if needed
    shader.SetVec4("spriteColor", glm::vec4(1.0f)); 
    GL_CHECK_ERROR("Uniform updates");

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices for 2 triangles
    glBindVertexArray(0);
    GL_CHECK_ERROR("glDrawArrays");

    m_texture->Unbind(0);
    GL_CHECK_ERROR("Texture unbind");

    // Restore previous OpenGL state
    if (wasDepthTestEnabled) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    if (!wasBlendEnabled) glDisable(GL_BLEND);
    else {
        // Restore specific blend func if it was more complex or different
        glBlendFuncSeparate(oldBlendSrcAlpha, oldBlendDstAlpha, oldBlendSrcAlpha, oldBlendDstAlpha); // Or simply glBlendFunc
        // Could also restore GL_BLEND_EQUATION if changed
    }
    
    if (!wasCullFaceEnabled) glDisable(GL_CULL_FACE);
    GL_CHECK_ERROR("Restore OpenGL state");
}

std::string Sprite::ShaderType() {
    return "sprite"; // This likely matches a shader file named "sprite.glsl" or similar
}
