#include "World/Camera3D.hpp"
#include "ECS/Entity.hpp"

Camera3D::Camera3D(const glm::vec3 &worldUp, float yaw, float pitch)
: WorldUp(worldUp), Yaw(yaw), Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    UpdateCameraVectors();
}

glm::mat4 Camera3D::GetViewMatrix() const {
    const glm::vec3 position = m_owner->GetTransform().GetGlobalPosition();
    return glm::lookAt(position, position + Front, Up);
}

void Camera3D::Update() {
    UpdateCameraVectors();
}

float Camera3D::GetYaw() const {
    return Yaw;
}

float Camera3D::GetPitch() const {
    return Pitch;
}

float Camera3D::GetZoom() const {
    return Zoom;
}

void Camera3D::SetYaw(float yaw) {
    Yaw = yaw;
}

void Camera3D::SetPitch(float pitch) {
    Pitch = pitch;
}

void Camera3D::SetZoom(float zoom) {
    Zoom = zoom;
}

glm::vec3 Camera3D::GetFront() const {
    return Front;
}

glm::vec3 Camera3D::GetRight() const {
    return Right;
}

void Camera3D::UpdateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // Re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
