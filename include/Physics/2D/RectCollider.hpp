#ifndef RECT_COLLIDER_HPP
#define RECT_COLLIDER_HPP

#include "ECS/Component.hpp"
#include "Graphics/2D/Sprite.hpp"
#include "Physics/Offset.hpp"
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
        Offset offset;

        std::vector<Entity*> CollidedEntities();

    private:
        Sprite* m_sprite;
        std::vector<Entity*> m_collidedEntities;
};

#endif
