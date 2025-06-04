#ifndef ANIMATION_2D_HPP
#define ANIMATION_2D_HPP

#include "World/Component.hpp"
#include "World/Entity.hpp"
#include "Core/Time.hpp"
#include "Graphics/Sprite.hpp"

class Animation2D : public Component {
    public:
        Animation2D(int width, int height, int currentRow, int framesCount, int animationSpeed);

        void Start() override;
        void Update() override;

    private:
        Sprite* m_sprite;
        int m_width;
        int m_height;
        float m_currentFrame;
        int m_currentRow;
        int m_framesCount;
        float m_animationSpeed;
};

#endif