#ifndef RIGIDBODY_2D_HPP
#define RIGIDBODY_2D_HPP

#include "ECS/Component.hpp"
#include "Physics/2D/RectCollider.hpp"

class Rigidbody2D : public Component {
    public:
        Rigidbody2D();
        ~Rigidbody2D();

        void Start() override;
        void Update() override;

        void SetVelocity(const glm::vec2& vel);
        glm::vec2 GetVelocity() const;

    private:
        RectCollider* m_rectCollider = nullptr;
        glm::vec2 m_velocity = glm::vec2(0.0f);
};

#endif