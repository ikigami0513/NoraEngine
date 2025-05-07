#include "World/Camera.hpp"
#include "World/Entity.hpp"

Camera::Camera(const glm::vec3 &worldUp, float yaw, float pitch)
: WorldUp(worldUp), Yaw(yaw), Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    const glm::vec3 position = m_owner->GetTransform().GetGlobalPosition();
    return glm::lookAt(position, position + Front, Up);
}

void Camera::Update() {
    UpdateCameraVectors();
}

float Camera::GetYaw() const {
    return Yaw;
}

float Camera::GetPitch() const {
    return Pitch;
}

float Camera::GetZoom() const {
    return Zoom;
}

void Camera::SetYaw(float yaw) {
    Yaw = yaw;
}

void Camera::SetPitch(float pitch) {
    Pitch = pitch;
}

void Camera::SetZoom(float zoom) {
    Zoom = zoom;
}

glm::vec3 Camera::GetFront() const {
    return Front;
}

glm::vec3 Camera::GetRight() const {
    return Right;
}

void Camera::UpdateCameraVectors() {
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
