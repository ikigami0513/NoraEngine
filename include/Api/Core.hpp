#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "Core/Time.hpp"
#include "Core/Window.hpp"

void bind_core(py::module_ m) {
    py::class_<Time>(m, "Time")
        .def_property_readonly_static("delta_time", [](py::object) { return Time::DeltaTime(); }, "Delta time between frames.")
        .def_property_readonly_static("fps", [](py::object) { return Time::FPS(); }, "Current frames per second.");

    py::enum_<WindowContext>(m, "WindowContext")
        .value("Context2D", WindowContext::Context2D)
        .value("Context3D", WindowContext::Context3D)
        .export_values();

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
        .def_property_static(
            "context",
            [](py::object) -> WindowContext {
                return Window::GetInstance().GetContext();
            },
            [](py::object, WindowContext new_context) {
                Window::GetInstance().SetContext(new_context);
            }
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
        })
        .def_property_static(
            "scene",
            [](py::object) -> Scene& {
                return Window::GetInstance().GetScene();
            },
            [](py::object, const Scene& scene) {
                Window::GetInstance().SetScene(scene);
            },
            "The current scene of the window."
        );
}
