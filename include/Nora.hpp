#include <pybind11/embed.h>
#include "Time.hpp"
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
}
