#include "Gui/Text.hpp"
#include "World/Entity.hpp"
#include "Core/Utils.hpp"

Text::Text() {}

Text::~Text() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
    }
}

void Text::Start() {
    SetupMesh();
}

void Text::SetupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Text::Render(Shader& shader) {
    GL_CHECK_ERROR("Text::Render - Start");

    // Save OpenGL state
    GLboolean wasDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean wasBlendEnabled = glIsEnabled(GL_BLEND);
    GLboolean wasCullFaceEnabled = glIsEnabled(GL_CULL_FACE);

    // Prepare for GUI rendering
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_CHECK_ERROR("Text::Render - After GL state setup for GUI");

    shader.Use();
    GL_CHECK_ERROR("Text::Render - After shader.Use()");

    shader.SetVec3("textColor", glm::vec3(color.r, color.g, color.b));
    GL_CHECK_ERROR("Text::Render - After SetVec3 textColor");

    shader.SetInt("text", 0); // Assurez-vous que votre classe Shader a SetInt
    GL_CHECK_ERROR("Text::Render - After SetInt text sampler");

    // Récupérer la position de base et l'échelle depuis le Transform de l'entité
    glm::vec3 owner_position = m_owner->GetTransform().GetLocalPosition();
    glm::vec3 owner_scale = m_owner->GetTransform().GetLocalScale();

    float cursor_x = owner_position.x; // Position x de départ du texte
    float baseline_y = owner_position.y; // Position y de la ligne de base du texte

    glActiveTexture(GL_TEXTURE0);
    GL_CHECK_ERROR("Text::Render - After glActiveTexture(GL_TEXTURE0)");

    glBindVertexArray(m_VAO);
    GL_CHECK_ERROR("Text::Render - After glBindVertexArray(m_VAO)");

    int char_index = 0;
    for (const char& char_code : this->text) {
        auto it = font->Characters.find(char_code);
        if (it == font->Characters.end()) {
            std::cerr << "[Text::Render] Loop " << char_index << ": Character '" << char_code << "' not found in font map." << std::endl;
            char_index++;
            continue;
        }

        const Character& ch = it->second;

        if (ch.TextureID == 0) {
            std::cerr << "[Text::Render] Loop " << char_index << ": ERROR - Character '" << char_code << "' has invalid TextureID 0." << std::endl;
            char_index++;
            continue;
        }

        // Calcul des positions basé sur l'exemple et l'échelle de l'entité
        float xpos = cursor_x + ch.Bearing.x * owner_scale.x;
        float ypos = baseline_y - (ch.Size.y - ch.Bearing.y) * owner_scale.y;
        float w = ch.Size.x * owner_scale.x;
        float h = ch.Size.y * owner_scale.y;

        // Mettre à jour les données du VBO
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        GL_CHECK_ERROR("Text::Render - Loop " + std::to_string(char_index) + " - After glBindTexture");

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        GL_CHECK_ERROR("Text::Render - Loop " + std::to_string(char_index) + " - After glBindBuffer for SubData");

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        GL_CHECK_ERROR("Text::Render - Loop " + std::to_string(char_index) + " - After glBufferSubData");

        if (w > 0 && h > 0) {
            glDrawArrays(GL_TRIANGLES, 0, 6);
            GL_CHECK_ERROR("Text::Render - Loop " + std::to_string(char_index) + " - After glDrawArrays");
        }

        cursor_x += (ch.Advance >> 6) * owner_scale.x;
        char_index++;
    }

    glBindVertexArray(0);
    GL_CHECK_ERROR("Text::Render - After unbinding VAO");

    glBindTexture(GL_TEXTURE_2D, 0);
    GL_CHECK_ERROR("Text::Render - After unbinding Texture");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GL_CHECK_ERROR("Text::Render - After unbinding VBO");


    // Restore previous OpenGL state
    if (wasDepthTestEnabled) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    if (!wasBlendEnabled) glDisable(GL_BLEND);
    if (!wasCullFaceEnabled) glDisable(GL_CULL_FACE); 
    GL_CHECK_ERROR("Text::Render - After restoring GL state");
}
