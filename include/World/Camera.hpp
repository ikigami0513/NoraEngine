#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "World/Component.hpp"
#include "Core/Time.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera : public Component {
    private:
        // Camera attributes
        glm::vec3 Front {0.0f, 0.0f, -1.0f };
        glm::vec3 Up {};
        glm::vec3 Right {};
        glm::vec3 WorldUp;

        // Euler angles
        float Yaw;
        float Pitch;

        // Camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        // Constants
        static constexpr float SPEED = 2.5f;
        static constexpr float SENSITIVITY = 0.1f;
        static constexpr float ZOOM = 45.0f;

        void UpdateCameraVectors();

    public:
        Camera(
            const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = -90.0f,
            float pitch = 0.0f
        );

        glm::mat4 GetViewMatrix() const;
        void ProcessKeyboard(CameraMovement direction);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void ProcessMouseScroll(float yoffset);
        float GetZoom() const;
};

#endif