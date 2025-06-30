#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "Core/Input/Input.hpp"
#include "Core/Input/Key.hpp"
#include "Core/Input/Mouse.hpp"

namespace py = pybind11;

void bind_input(py::module_ m) {
    py::enum_<Key>(m, "Key")
        .value("Unknown", Key::Unknown)
        .value("Space", Key::Space)
        .value("Apostrophe", Key::Apostrophe)
        .value("Comma", Key::Comma)
        .value("Minus", Key::Minus)
        .value("Period", Key::Period)
        .value("Slash", Key::Slash)
        .value("Num0", Key::Num0)
        .value("Num1", Key::Num1)
        .value("Num2", Key::Num2)
        .value("Num3", Key::Num3)
        .value("Num4", Key::Num4)
        .value("Num5", Key::Num5)
        .value("Num6", Key::Num6)
        .value("Num7", Key::Num7)
        .value("Num8", Key::Num8)
        .value("Num9", Key::Num9)
        .value("A", Key::A)
        .value("B", Key::B)
        .value("C", Key::C)
        .value("D", Key::D)
        .value("E", Key::E)
        .value("F", Key::F)
        .value("G", Key::G)
        .value("H", Key::H)
        .value("I", Key::I)
        .value("J", Key::J)
        .value("K", Key::K)
        .value("L", Key::L)
        .value("M", Key::M)
        .value("N", Key::N)
        .value("O", Key::O)
        .value("P", Key::P)
        .value("Q", Key::Q)
        .value("R", Key::R)
        .value("S", Key::S)
        .value("T", Key::T)
        .value("U", Key::U)
        .value("V", Key::V)
        .value("W", Key::W)
        .value("X", Key::X)
        .value("Y", Key::Y)
        .value("Z", Key::Z)
        .value("Escape", Key::Escape)
        .value("Enter", Key::Enter)
        .value("Tab", Key::Tab)
        .value("Backspace", Key::Backspace)
        .value("Insert", Key::Insert)
        .value("Delete", Key::Delete)
        .value("Right", Key::Right)
        .value("Left", Key::Left)
        .value("Down", Key::Down)
        .value("Up", Key::Up)
        .export_values();

    py::enum_<MouseMode>(m, "MouseMode")
        .value("Normal", MouseMode::NORMAL)
        .value("Hidden", MouseMode::HIDDEN)
        .value("Disabled", MouseMode::DISABLED)
        .export_values();

    py::class_<Mouse>(m, "Mouse")
        .def_property_static("mode",
            [](py::object /* cls */) { return Mouse::GetMode(); },      // Getter
            [](py::object /* cls */, MouseMode mode) { Mouse::SetMode(mode); } // Setter
        );

    py::enum_<MouseCode>(m, "MouseCode")
        .value("Button1", MouseCode::Button1)
        .value("Button2", MouseCode::Button2)
        .value("Button3", MouseCode::Button3)
        .value("Button4", MouseCode::Button4)
        .value("Button5", MouseCode::Button5)
        .value("Button6", MouseCode::Button6)
        .value("Button7", MouseCode::Button7)
        .value("Button8", MouseCode::Button8)
        .value("Left", MouseCode::Left)
        .value("Right", MouseCode::Right)
        .value("Middle", MouseCode::Middle)
        .export_values();

    py::class_<Input>(m, "Input")
        .def_static("is_key_pressed", &Input::IsKeyPressed, py::arg("key"), "Returns True if the specified key is currently pressed.")
        .def_static("is_just_pressed", &Input::IsJustPressed, py::arg("key"), "Returns True if the specified key is just pressed.")
        .def_static("is_just_released", &Input::IsJustReleased, py::arg("key"), "Returns True if the specified key is just released.")
        .def_static("is_mouse_button_pressed", &Input::IsMouseButtonPressed, py::arg("mouse_code"), "Returns True if the specified mouse button is currently pressed.")
        .def_static("is_mouse_button_just_pressed", &Input::IsMouseButtonJustPressed, py::arg("mouse_code"), "Returns True if the specified mouse button is just pressed.")
        .def_static("is_mouse_button_just_released", &Input::IsMouseButtonJustReleased, py::arg("mouse_code"), "Returns True if the specified mouse button is just released.")
        .def_property_readonly_static(
            "mouse_position",
            [](py::object) {
                return Input::GetMousePosition();
            },
            "Returns current mouse position."
        )
        .def_property_readonly_static(
            "mouse_delta",
            [](py::object) {
                return Input::GetMouseDelta();
            },
            "Returns current mouse delta."
        )
        .def_property_readonly_static(
            "scroll_delta",
            [](py::object) {
                return Input::GetScrollDelta();
            },
            "Returns current mouse scroll delta."
        );
}
