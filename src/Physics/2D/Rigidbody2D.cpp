#include "Physics/2D/Rigidbody2D.hpp"
#include "ECS/Entity.hpp"
#include "Core/Time.hpp"

Rigidbody2D::Rigidbody2D() {}

Rigidbody2D::~Rigidbody2D() {}

void Rigidbody2D::Start() {
    m_rectCollider = m_owner->GetComponent<RectCollider>();
}

void Rigidbody2D::SetVelocity(const glm::vec2& vel) {
    m_velocity = vel;
}

glm::vec2 Rigidbody2D::GetVelocity() const {
    return m_velocity;
}

void Rigidbody2D::Update() {
    if (!m_rectCollider) return;

    Transform& transform = m_owner->GetTransform();
    glm::vec3 position = transform.GetLocalPosition();
    position += glm::vec3(m_velocity * Time::DeltaTime(), 0.0f);
    transform.SetLocalPosition(position);

    if (m_rectCollider->CollidedEntities().empty()) return;

    glm::vec4 myBounds = m_rectCollider->GetBounds();

    for (Entity* other : m_rectCollider->CollidedEntities()) {
        RectCollider* otherCollider = other->GetComponent<RectCollider>();
        if (!otherCollider) continue;

        glm::vec4 otherBounds = otherCollider->GetBounds();

        float overlapX1 = myBounds.z - otherBounds.x;
        float overlapX2 = otherBounds.z - myBounds.x;
        float penetrationX = std::min(overlapX1, overlapX2);

        float overlapY1 = myBounds.w - otherBounds.y;
        float overlapY2 = otherBounds.w - myBounds.y;
        float penetrationY = std::min(overlapY1, overlapY2);

        glm::vec2 mtv(0.0f);

        if (penetrationX < penetrationY) {
            float dir = (myBounds.x + myBounds.z < otherBounds.x + otherBounds.z) ? -1.0f : 1.0f;
            mtv.x = dir * penetrationX;

            // Annule la vélocité horizontale
            m_velocity.x = 0.0f;
        }
        else {
            float dir = (myBounds.y + myBounds.w < otherBounds.y + otherBounds.w) ? -1.0f : 1.0f;
            mtv.y = dir * penetrationY;

            // Annule la vélocité verticale
            m_velocity.y = 0.0f;
        }

        // Appliquer la correction
        glm::vec3 correctedPos = transform.GetLocalPosition();
        correctedPos += glm::vec3(mtv, 0.0f);
        transform.SetLocalPosition(correctedPos);

        // Met à jour les bounds pour le prochain test éventuel
        myBounds = m_rectCollider->GetBounds();
    }
}