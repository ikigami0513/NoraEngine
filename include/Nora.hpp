#include <pybind11/embed.h>
#include "Time.hpp"
#include "Input.hpp"
#include "Key.hpp"
#include "Graphics/Color.hpp"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(nora, m) {
    py::class_<Color>(m, "Color")
        .def(
            py::init<float, float, float, float>(),
            py::arg("r") = 0.0f,
            py::arg("g") = 0.0f,
            py::arg("b") = 0.0f,
            py::arg("alpha") = 1.0f
        )
        .def_readwrite("r", &Color::r)
        .def_readwrite("g", &Color::g)
        .def_readwrite("b", &Color::b)
        .def_readwrite("alpha", &Color::alpha)
        .def("__repr__", [](const Color& c) {
            return "<Color r=" + std::to_string(c.r) +
                " g=" + std::to_string(c.g) +
                " b=" + std::to_string(c.b) +
                " alpha=" + std::to_string(c.alpha) + ">";
        });

    py::class_<Time>(m, "Time")
        .def_property_readonly_static("delta_time", [](py::object) { return Time::DeltaTime(); }, "Delta time between frames.")
        .def_property_readonly_static("fps", [](py::object) { return Time::FPS(); }, "Current frames per second.");

    py::class_<Window>(m, "Window", py::module_local())
        .def_property_static(
            "background_color",
            [](py::object) -> Color& {
                return Window::GetInstance().BackgroundColor;
            },
            [](py::object, const Color& color) {
                Window::GetInstance().BackgroundColor = color;
            },
            "The background color of the window."
        )
        .def_static("set_title", [](const std::string& title) {
            Window::GetInstance().SetTitle(title);
        })
        .def_static("get_title", []() {
            return Window::GetInstance().GetTitle();
        })
        .def_static("get_size", []() {
            return Window::GetInstance().GetSize();
        })
        .def_static("set_size", [](int width, int height) {
            Window::GetInstance().SetSize(width, height);
        });

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
        
    py::class_<Input>(m, "Input")
        .def_static("is_key_pressed", &Input::IsKeyPressed, py::arg("key"), "Returns True if the specified key is currently pressed.")
        .def_static("is_just_pressed", &Input::IsJustPressed, py::arg("key"), "Returns True if the specified key is just pressed.")
        .def_static("is_just_released", &Input::IsJustReleased, py::arg("key"), "Returns True if the specified key is just released.");
}
