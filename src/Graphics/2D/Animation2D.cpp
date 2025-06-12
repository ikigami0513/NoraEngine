#include "Graphics/2D/Animation2D.hpp"

Animation2D::Animation2D(int width, int height, int currentRow, int framesCount, int animationSpeed, bool repeat) {
    m_width = width;
    m_height = height;
    m_currentRow = currentRow;
    m_framesCount = framesCount;
    m_animationSpeed = animationSpeed;
    m_repeat = repeat;
    m_finish = false;
}

void Animation2D::Start() {
    m_sprite = m_owner->GetComponent<Sprite>();
}

void Animation2D::Update() {
    if (!m_finish) {
        m_currentFrame += m_animationSpeed * Time::DeltaTime();
        if (static_cast<int>(m_currentFrame) >= m_framesCount) {
            if (m_repeat) {
                m_currentFrame = 0.0f;
            }
            else {
                m_finish = true;
            }
        }

        m_sprite->SetTextureRect(
            static_cast<int>(m_currentFrame) * m_width,
            (m_currentRow - 1) * m_height,
            m_width,
            m_height
        );
    }
}

void Animation2D::SetRepeat(bool repeat) {
    m_repeat = repeat;
}

bool Animation2D::GetRepeat() {
    return m_repeat;
}

void Animation2D::SetFinish(bool finish) {
    m_finish = finish;
}

bool Animation2D::GetFinish() {
    return m_finish;
}

void Animation2D::SetCurrentFrame(float currentFrame) {
    m_currentFrame = currentFrame;
}

float Animation2D::GetCurrentFrame() {
    return m_currentFrame;
}

void Animation2D::SetFramesCount(int framesCount) {
    m_framesCount = framesCount;
}

int Animation2D::GetFramesCount() {
    return m_framesCount;
}