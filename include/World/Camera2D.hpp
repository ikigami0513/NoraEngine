#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

#include "World/Component.hpp"
#include "Core/Time.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D : public Component {
    private:
        float m_zoom = 1.0f;
        
    public:
        void SetZoom(float zoom);
        float GetZoom() const;

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
};

#endif