#include "Physics/2D/Rigidbody2D.hpp"
#include "ECS/Entity.hpp"

Rigidbody2D::Rigidbody2D() {}

Rigidbody2D::~Rigidbody2D() {}

void Rigidbody2D::Start() {
    m_rectCollider = m_owner->GetComponent<RectCollider>();
}

void Rigidbody2D::Update() {
    if (!m_rectCollider) return;
    if (m_rectCollider->CollidedEntities().empty()) return;

    glm::vec4 myBounds = m_rectCollider->GetBounds();

    for (Entity* other : m_rectCollider->CollidedEntities()) {
        glm::vec4 otherBounds = other->GetComponent<RectCollider>()->GetBounds();

        float penetrationX = std::min(myBounds.z - otherBounds.x, otherBounds.z - myBounds.x);
        float penetrationY = std::min(myBounds.w - otherBounds.y, otherBounds.w - myBounds.y);

        Transform& transform = m_owner->GetTransform();
        glm::vec3 position = transform.GetLocalPosition();

        if (penetrationX < penetrationY) {
            float myCenterX = (myBounds.x + myBounds.z) / 2.0f;
            float otherCenterX = (otherBounds.x + otherBounds.z) / 2.0f;

            if (myCenterX < otherCenterX) {
                position.x -= penetrationX;
            }
            else {
                position.x += penetrationX;
            }
        }
        else {
            float myCenterY = (myBounds.y + myBounds.w) / 2.0f;
            float otherCenterY = (otherBounds.y + otherBounds.w) / 2.0f;

            if (myCenterY < otherCenterY) {
                position.y -= penetrationY;
            }
            else {
                position.y += penetrationY;
            }
        }

        transform.SetLocalPosition(position);
    }
}