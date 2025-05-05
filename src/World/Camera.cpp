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

void Camera::ProcessKeyboard(CameraMovement direction) {
    Transform& transform = m_owner->GetTransform();
    glm::vec3 position = transform.GetLocalPosition();

    float velocity = MovementSpeed * Time::DeltaTime();
    if (direction == FORWARD) {
        position += Front * velocity;
    }
    if (direction == BACKWARD) {
        position -= Front * velocity;
    }
    if (direction == LEFT) {
        position -= Right * velocity;
    }
    if (direction == RIGHT) {
        position += Right * velocity;
    }

    transform.SetLocalPosition(position);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f) {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f) {
            Pitch = -89.0f;
        }
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= yoffset;
    Zoom = glm::clamp(Zoom, 1.0f, 45.0f);
}

float Camera::GetZoom() const {
    return Zoom;
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
