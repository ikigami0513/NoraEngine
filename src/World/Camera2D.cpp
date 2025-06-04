#include "World/Camera2D.hpp"
#include "World/Entity.hpp"
#include "Core/Window.hpp"

void Camera2D::SetZoom(float zoom) {
    m_zoom = zoom;
}

float Camera2D::GetZoom() const {
    return m_zoom;
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