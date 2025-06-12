#include "Physics/2D/RectCollider.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene.hpp"
#include "Core/Window.hpp"
#include "Core/Time.hpp"
#include "Utils/Debug.hpp"

RectCollider::RectCollider() {}

RectCollider::~RectCollider() {}

void RectCollider::Start() {
    m_sprite = m_owner->GetComponent<Sprite>();
}

void RectCollider::Update() {
    m_collidedEntities.clear();
    Scene scene = Window::GetInstance().GetScene();
    
    std::vector<Entity*> collidableEntities = scene.GetEntitiesWithComponent<RectCollider>();
    glm::vec4 bounds = GetBounds();

    for (Entity* other : collidableEntities) {
        if (other == m_owner) {
            continue;
        }

        RectCollider* otherCollider = other->GetComponent<RectCollider>();
        if (!otherCollider) {
            continue;
        }

        glm::vec4 otherBounds = otherCollider->GetBounds();

        bool collisionX = bounds.x <= otherBounds.z && bounds.z >= otherBounds.x;
        bool collisionY = bounds.y <= otherBounds.w && bounds.w >= otherBounds.y;

        if (collisionX && collisionY) {
            m_collidedEntities.push_back(other);
            m_owner->OnCollisionEnter(other);
        }
    }
}

glm::vec4 RectCollider::GetBounds() const {
    if (!m_owner) {
        return glm::vec4(0.0f);
    }

    const Transform& transform = m_owner->GetTransform();
    glm::vec3 position = transform.GetGlobalPosition();
    glm::vec3 scale = transform.GetGlobalScale();
    glm::vec3 size;

    if (m_sprite && m_sprite->GetTexture()) { // Ajout d'une vérification sur la texture
        glm::vec4 texRect = m_sprite->GetTextureRect(); // (u_min, v_min, u_max, v_max)
        float normalizedWidth = texRect.z - texRect.x; // u_max - u_min
        float normalizedHeight = texRect.w - texRect.y; // v_max - v_min

        float width = m_sprite->GetTexture()->Width() * normalizedWidth * scale.x;
        float height = m_sprite->GetTexture()->Height() * normalizedHeight * scale.y;

        size = glm::vec3(width, height, 0.0f);
    }
    else {
        size = glm::vec3(scale.x, scale.y, 0.0f);
    }

    glm::vec3 halfSize = size / 2.0f;
    glm::vec3 min = position - halfSize;
    glm::vec3 max = position + halfSize;

    min.x += offset.left;
    min.y += offset.down;
    max.x -= offset.right;
    max.y -= offset.up;

    return glm::vec4(min.x, min.y, max.x, max.y);
}

std::vector<Entity*> RectCollider::CollidedEntities() {
    return m_collidedEntities;
}