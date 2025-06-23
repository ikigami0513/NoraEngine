#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <memory>
#include <glm/glm.hpp> // Include for glm::vec4

#include "ECS/Component.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

class Sprite : public Component {
public:
    Sprite();
    ~Sprite();

    void Start() override;
    void Render(Shader& shader);
    std::string ShaderType();

    void SetTexture(std::shared_ptr<Texture> texture) {
        m_texture = std::move(texture);
        if (m_texture) {
            m_color = glm::vec4(1.0f);
        }
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

    // --- NOUVEAU: Méthodes pour la Couleur ---
    /**
     * @brief Définit la couleur du sprite.
     * Si une texture est présente, cette couleur agira comme une teinte.
     * Si aucune texture n'est définie, le sprite sera un rectangle plein de cette couleur.
     * @param color La couleur au format RGBA.
     */
    void SetColor(const glm::vec4& color) { m_color = color; }

    /**
     * @brief Récupère la couleur actuelle du sprite.
     * @return La couleur au format RGBA.
     */
    const glm::vec4& GetColor() const { return m_color; }

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
    glm::vec4 m_color;
};

#endif