#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "Gui/Font.hpp"
#include "Gui/GuiComponent.hpp"
#include "Gui/Text.hpp"
#include "Gui/Rectangle.hpp"
#include "Gui/Button.hpp"
#include "Physics/Offset.hpp"

namespace py = pybind11;

void bind_gui(py::module_ m) {
    py::class_<Font, std::shared_ptr<Font>>(m, "Font")
        .def(py::init<const std::string&, unsigned int>(), py::arg("font_path"), py::arg("font_size") = 48);

    py::class_<GuiComponent, Component, std::shared_ptr<GuiComponent>>(m, "GuiComponent");

    py::enum_<Alignment>(m, "Alignment")
        .value("Left", Alignment::Left)
        .value("Center", Alignment::Center)
        .value("Right", Alignment::Right)
        .export_values();

    py::class_<Text, GuiComponent, std::shared_ptr<Text>>(m, "Text")
        .def(py::init<>())
        .def_property("font",
            [](Text& self) -> std::shared_ptr<Font> { // Getter
                return self.font; // Retourne directement le shared_ptr
            },
            [](Text& self, std::shared_ptr<Font> new_font) { // Setter
                self.font = new_font; // Assignation directe du shared_ptr
            },
            "The font used by the text component.")
        .def_property("text",
            [](const Text& self) { return self.text; },
            [](Text& self, const std::string& new_text) { self.text = new_text; },
            "The content of the text.")
        .def_property("color",
            [](const Text& self) { return self.color; },
            [](Text& self, const Color& new_color) { self.color = new_color; },
            "The color of the text.")
        .def_property("margin",
            [](const Text& self) { return self.margin; },
            [](Text& self, const float new_margin) { self.margin = new_margin; }
        )
        .def_property("alignement",
            [](const Text& self) { return self.alignmenent; },
            [](Text& self, Alignment new_alignement) { self.alignmenent = new_alignement; }
        )
        .def("get_text_bounds", &Text::GetTextBounds, py::arg("scale") = 1.0f);

    py::class_<Rectangle, GuiComponent, std::shared_ptr<Rectangle>>(m, "Rectangle")
        .def(py::init<Color, float, float>(), py::arg("color") = Color(1.0f, 1.0f, 1.0f, 1.0f), py::arg("width") = 100.0f, py::arg("height") = 50.0f)
        .def_property(
            "color",
            &Rectangle::GetColor,
            &Rectangle::SetColor
        )
        .def_property(
            "width",
            &Rectangle::GetWidth,
            &Rectangle::SetWidth
        )
        .def_property(
            "height",
            &Rectangle::GetHeight,
            &Rectangle::SetHeight
        );

    py::class_<Offset>(m, "Offset")
        .def(py::init<float, float, float, float>(), py::arg("up") = 0.0f, py::arg("down") = 0.0f, py::arg("left") = 0.0f, py::arg("right") = 0.0f)
        .def_property("up", [](const Offset& offset) { return offset.up; }, [](Offset& offset, float p_up) { offset.up = p_up; })
        .def_property("down", [](const Offset& offset) { return offset.down; }, [](Offset& offset, float p_down) { offset.down = p_down; })
        .def_property("left", [](const Offset& offset) { return offset.left; }, [](Offset& offset, float p_left) { offset.left = p_left; })
        .def_property("right", [](const Offset& offset) { return offset.right; }, [](Offset& offset, float p_right) { offset.right = p_right; });

    py::class_<Button, Component, std::shared_ptr<Button>>(m, "Button")
        .def(py::init<>())
        .def_property("hovered_color",
            [](const Button& self) { return self.m_hoveredColor; },
            [](Button& self, const Color& color) { self.m_hoveredColor = color; },
            "La couleur du bouton lorsqu'il est survolé par la souris.")
        .def_property("on_click_color",
            [](const Button& self) { return self.m_onClickColor; },
            [](Button& self, const Color& color) { self.m_onClickColor = color; },
            "La couleur du bouton lorsqu'il est cliqué.")
        .def_property("on_click",
            nullptr,
            [](Button& self, const std::function<void()>& func) {
                self.OnClick = func;
            }
        );
}
