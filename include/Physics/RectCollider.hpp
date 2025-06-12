#ifndef RECT_COLLIDER_HPP
#define RECT_COLLIDER_HPP

#include "World/Component.hpp"
#include "Graphics/Sprite.hpp"
#include <glm/glm.hpp>
#include <functional>

class Entity;

class RectCollider : public Component {
    public:
        RectCollider();
        virtual ~RectCollider();

        void Start() override;
        void Update() override;

        glm::vec4 GetBounds() const;

    private:
        Sprite* m_sprite;
};

#endif
