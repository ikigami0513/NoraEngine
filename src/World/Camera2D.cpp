#include "World/Camera2D.hpp"
#include "ECS/Entity.hpp"
#include "Core/Window.hpp"

void Camera2D::SetZoom(float zoom) {
    m_zoom = zoom;
}

float Camera2D::GetZoom() const {
    return m_zoom;
}

void Camera2D::SetTarget(Entity* target) {
    m_target = target;
}

Entity* Camera2D::GetTarget() {
    return m_target;
}

glm::mat4 Camera2D::GetViewMatrix() const {
    return glm::translate(glm::mat4(1.0f), glm::vec3(-m_owner->GetTransform().GetLocalPosition()));
}

glm::mat4 Camera2D::GetProjectionMatrix() const {
    const std::pair<int, int> size = Window::GetInstance().GetSize();

    return glm::ortho(
        0.0f, static_cast<float>(size.first), 
        0.0f, static_cast<float>(size.second), 
        -1.0f, 1.0f
    );
}

void Camera2D::Update() {
    if (m_target != nullptr) {
        glm::vec3 targetPos = m_target->GetTransform().GetLocalPosition();
        std::pair<int, int> size = Window::GetInstance().GetSize();
        m_owner->GetTransform().SetLocalPosition(glm::vec3(targetPos.x - (size.first / 2), targetPos.y - (size.second / 2), 0.0f));
    }
}