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

    private:
        RectCollider* m_rectCollider;
};

#endif