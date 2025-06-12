#include "Gui/Button.hpp"
#include "Core/Input/Input.hpp"
#include "Core/Window.hpp"

Button::Button() {}

Button::~Button() {}

void Button::Start() {
    m_text = m_owner->GetComponent<Text>();
    m_rectangle = m_owner->GetComponent<Rectangle>();

    m_normalColor = m_rectangle->GetColor();
}

void Button::Update() {
    if (!m_rectangle || !m_owner) {
        return;
    }

    std::pair<double, double> mousePosition = Input::GetMousePosition();

    std::pair<int, int> windowSize = Window::GetInstance().GetSize();
    float windowHeight = static_cast<float>(windowSize.second);

    mousePosition.second = windowHeight - mousePosition.second;

    Transform& transform = m_owner->GetTransform();
    glm::vec3 pos = transform.GetLocalPosition();
    glm::vec3 scale = transform.GetLocalScale();

    glm::vec2 finalSize = glm::vec2(
        m_rectangle->GetWidth() * scale.x,
        m_rectangle->GetHeight() * scale.y
    );

    float left   = pos.x - finalSize.x / 2.0f;
    float right  = pos.x + finalSize.x / 2.0f;
    float bottom = pos.y - finalSize.y / 2.0f;
    float top    = pos.y + finalSize.y / 2.0f;

    if (
        mousePosition.first >= left && mousePosition.first <= right &&
        mousePosition.second >= bottom && mousePosition.second <= top
    ) {
        if (Input::IsMouseButtonPressed(MouseCode::Left)) {
            m_rectangle->SetColor(m_onClickColor);

            if (Input::IsMouseButtonJustPressed(MouseCode::Left)) {
                if (OnClick && !m_exec) {
                    OnClick();
                    m_exec = true;
                }
            }
        }
        else {
            m_rectangle->SetColor(m_hoveredColor);
            m_exec = false;
        }
    }
    else {
        m_rectangle->SetColor(m_normalColor);
        m_exec = false;
    }
}
