#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <memory>
#include <glm/glm.hpp>
#include <array>

#include "ECS/Component.hpp"
#include "ECS/Entity.hpp"
#include "Graphics/Texture.hpp"
#include "Utils/Math.hpp"

struct SpriteVertex {
    glm::vec3 position; // x, y, z
    glm::vec2 texCoords; // u, v
    glm::vec4 color;
};

class Sprite : public Component {
    public:
        Sprite();
        ~Sprite();

        void Start() override;
        std::string ShaderType();

        void SetTexture(std::shared_ptr<Texture> texture) {
            m_texture = std::move(texture);
            if (m_texture) {
                m_color = glm::vec4(1.0f);
            }
        }

        std::shared_ptr<Texture> GetTexture() const {
            return m_texture;
        }

        void SetTextureRect(float pixel_x, float pixel_y, float pixel_width, float pixel_height);
        glm::vec4 GetTextureRect() const;

        void SetColor(const glm::vec4& color) { m_color = color; }
        const glm::vec4& GetColor() const { return m_color; }

        AABB GetWorldAABB() const;

        std::array<SpriteVertex, 6> GetVerticesData() const;

    private:
        void InitializeVertices(float u_min, float v_min, float u_max, float v_max);

        std::array<SpriteVertex, 6> m_localVertices;

        std::shared_ptr<Texture> m_texture;
        glm::vec4 m_currentTextureRectNormalized;
        glm::vec4 m_color;
};

#endif