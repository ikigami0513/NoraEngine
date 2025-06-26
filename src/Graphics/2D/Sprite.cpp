#include "Graphics/2D/Sprite.hpp"
#include "ECS/Entity.hpp"
#include "Utils/Debug.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

void Sprite::InitializeVertices(float u_min, float v_min, float u_max, float v_max) {
    // Les positions locales d'un quad unitaire (-0.5 à 0.5)
    // Sont fixes pour tous les sprites, le scaling sera appliqué par la matrice modèle.
    // L'ordre des sommets doit correspondre à celui utilisé pour l'indexage ou le dessin non-indexé.

    // Triangle 1: (BL, BR, TR)
    m_localVertices[0].position = glm::vec3(-0.5f, -0.5f, 0.0f); // BL
    m_localVertices[1].position = glm::vec3( 0.5f, -0.5f, 0.0f); // BR
    m_localVertices[2].position = glm::vec3( 0.5f,  0.5f, 0.0f); // TR

    // Triangle 2: (TR, TL, BL)
    m_localVertices[3].position = glm::vec3( 0.5f,  0.5f, 0.0f); // TR
    m_localVertices[4].position = glm::vec3(-0.5f,  0.5f, 0.0f); // TL
    m_localVertices[5].position = glm::vec3(-0.5f, -0.5f, 0.0f); // BL

    // Coordonnées de texture
    // Triangle 1: (BL, BR, TR)
    m_localVertices[0].texCoords = glm::vec2(u_min, v_min);
    m_localVertices[1].texCoords = glm::vec2(u_max, v_min);
    m_localVertices[2].texCoords = glm::vec2(u_max, v_max);

    // Triangle 2: (TR, TL, BL)
    m_localVertices[3].texCoords = glm::vec2(u_max, v_max);
    m_localVertices[4].texCoords = glm::vec2(u_min, v_max);
    m_localVertices[5].texCoords = glm::vec2(u_min, v_min);

    // Les couleurs des sommets seront mises à jour dans GetVerticesData en fonction de m_color.
}

Sprite::Sprite() : m_color(1.0f, 1.0f, 1.0f, 1.0f) {
    // Initialize with default texture coordinates (full texture)
    InitializeVertices(0.0f, 0.0f, 1.0f, 1.0f);
}

Sprite::~Sprite() {}

void Sprite::Start() {
    // Si une texture est déjà définie au moment de Start(), on peut configurer le rect.
    // Sinon, le rect par défaut (texture entière) est déjà appliqué dans le constructeur.
    if (m_texture) {
        SetTextureRect(0.0f, 0.0f, m_texture->Width(), m_texture->Height());
    }
}

void Sprite::SetTextureRect(float pixel_x, float pixel_y, float pixel_width, float pixel_height) {
    if (!m_texture) {
        Debug::Warning("Sprite::SetTextureRect - Texture non définie. Impossible de calculer les UVs à partir des pixels.");
        InitializeVertices(0.0f, 0.0f, 1.0f, 1.0f); // Fallback sur la texture entière
        return;
    }

    float texW = static_cast<float>(m_texture->Width());
    float texH = static_cast<float>(m_texture->Height());

    if (texW == 0 || texH == 0) {
        Debug::Warning("Sprite::SetTextureRect - La texture a des dimensions nulles. Impossible de calculer les UVs.");
        InitializeVertices(0.0f, 0.0f, 1.0f, 1.0f); // Fallback
        m_currentTextureRectNormalized = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
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
}

glm::vec4 Sprite::GetTextureRect() const {
    return m_currentTextureRectNormalized;
}

std::array<SpriteVertex, 6> Sprite::GetVerticesData() const {
    std::array<SpriteVertex, 6> batchedVertices;
    glm::mat4 modelMatrix = m_owner->GetTransform().GetLocalModelMatrix2D(m_texture, m_currentTextureRectNormalized);

    for (int i = 0; i < 6; i++) {
        // Transforme la position locale du sommet par la matrice modèle de l'entité
        glm::vec4 transformedPos = modelMatrix * glm::vec4(m_localVertices[i].position, 1.0f);
        batchedVertices[i].position = glm::vec3(transformedPos);

        // Copie les coordonnées de texture
        batchedVertices[i].texCoords = m_localVertices[i].texCoords;

        // Copie la couleur du sprite pour tous les sommets
        batchedVertices[i].color = m_color;
    }

    return batchedVertices;
}

std::string Sprite::ShaderType() {
    return "sprite";
}

AABB Sprite::GetWorldAABB() const {
    // Calcul de l'AABB mondiale basé sur la transformation de l'entité et les dimensions de la texture.
    // Si la taille du sprite est indépendante de la texture (e.g. toujours un quad unitaire),
    // alors la logique de GetLocalModelMatrix2D devrait gérer le scaling.
    // Ici, on va estimer l'AABB en utilisant la position et les dimensions de la texture (si présente).
    // Si pas de texture, on peut prendre une taille par défaut (ex: 1x1).

    glm::vec3 position = m_owner->GetTransform().GetLocalPosition();
    glm::vec3 scale = m_owner->GetTransform().GetLocalScale();
    glm::vec2 pos = { position.x, position.y };
    glm::vec2 size;

    if (m_texture) {
        size = { m_texture->Width(), m_texture->Height() };
    }
    else {
        size = { 1.0f, 1.0f };
    }

    size.x *= scale.x;
    size.y *= scale.y;

    glm::vec2 halfSize = size * 0.5f;
    return { pos - halfSize, pos + halfSize };
}