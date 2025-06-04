#include "Graphics/Animation2D.hpp"

Animation2D::Animation2D(int width, int height, int currentRow, int framesCount, int animationSpeed) {
    m_width = width;
    m_height = height;
    m_currentRow = currentRow;
    m_framesCount = framesCount;
    m_animationSpeed = animationSpeed;
}

void Animation2D::Start() {
    m_sprite = m_owner->GetComponent<Sprite>();
}

void Animation2D::Update() {
    m_currentFrame += m_animationSpeed * Time::DeltaTime();
    if (static_cast<int>(m_currentFrame) >= m_framesCount) {
        m_currentFrame = 0.0f;
    }

    m_sprite->SetTextureRect(
        static_cast<int>(m_currentFrame) * m_width,
        (m_currentRow - 1) * m_height,
        m_width,
        m_height
    );
}