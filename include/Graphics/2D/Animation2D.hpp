#ifndef ANIMATION_2D_HPP
#define ANIMATION_2D_HPP

#include "ECS/Component.hpp"
#include "ECS/Entity.hpp"
#include "Core/Time.hpp"
#include "Graphics/2D/Sprite.hpp"

class Animation2D : public Component {
    public:
        Animation2D(int width, int height, int currentRow, int framesCount, int animationSpeed, bool repeat = true);

        void Start() override;
        void Update() override;
        
        void SetRepeat(bool repeat);
        bool GetRepeat();

        void SetFinish(bool finish);
        bool GetFinish();

        void SetCurrentFrame(float currentFrame);
        float GetCurrentFrame();

        void SetFramesCount(int framesCount);
        int GetFramesCount();

    private:
        Sprite* m_sprite;
        int m_width;
        int m_height;
        float m_currentFrame;
        int m_currentRow;
        int m_framesCount;
        float m_animationSpeed;
        bool m_repeat;
        bool m_finish;
};

#endif