#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <memory>
#include <glm/glm.hpp> // Include for glm::vec4

#include "World/Component.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

class Sprite : public Component {
public:
    Sprite();
    ~Sprite();

    void Start() override;
    void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    std::string ShaderType();

    void SetTexture(std::shared_ptr<Texture> texture) {
        m_texture = std::move(texture);
    }

    std::shared_ptr<Texture> GetTexture() {
        return m_texture;
    }

    /**
     * @brief Définit la région rectangulaire de la texture à afficher, en utilisant des coordonnées en pixels.
     * @param pixel_x Coordonnée X du coin supérieur gauche de la sous-région (en pixels).
     * @param pixel_y Coordonnée Y du coin supérieur gauche de la sous-région (en pixels).
     * @param pixel_width Largeur de la sous-région (en pixels).
     * @param pixel_height Hauteur de la sous-région (en pixels).
     */
    void SetTextureRect(float pixel_x, float pixel_y, float pixel_width, float pixel_height);

    glm::vec4 GetTextureRect() const;

private:
    void SetupMesh();
    /**
     * @brief Helper to initialize or update m_vertices with new texture coordinates.
     */
    void InitializeVertices(float u_min, float v_min, float u_max, float v_max);

    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;

    float m_vertices[30]; // Non-static, stores positions and UVs for this sprite
    std::shared_ptr<Texture> m_texture;
    // Optional: Store the current rect if needed for other logic
    // glm::vec4 m_currentTextureRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 m_currentTextureRectNormalized;
};

#endif