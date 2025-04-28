#include <pybind11/embed.h>
#include "Time.hpp"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(nora_api, m) {
    py::class_<Time>(m, "Time")
        .def_property_readonly_static("delta_time", [](py::object) { return Time::DeltaTime(); }, "Delta time between frames.")
        .def_property_readonly_static("fps", [](py::object) { return Time::FPS(); }, "Current frames per second.");
}