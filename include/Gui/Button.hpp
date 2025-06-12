#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "World/Component.hpp"
#include "Gui/Text.hpp"
#include "Gui/Rectangle.hpp"
#include "World/Entity.hpp"
#include "Graphics/Color.hpp"
#include <functional>

class Button : public Component {
    public:
        Button();
        ~Button();

        void Start() override;
        void Update() override;

        Color m_hoveredColor;
        Color m_onClickColor;

        std::function<void()> OnClick;

    private:
        Text* m_text;
        Rectangle* m_rectangle;
        Color m_normalColor;
        bool m_exec = false;
};

#endif