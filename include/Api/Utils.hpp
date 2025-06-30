#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "Utils/Debug.hpp"

namespace py = pybind11;

void bind_utils(py::module_ m) {
    py::class_<Debug>(m, "Debug")
        .def_static("info", &Debug::Info, py::arg("message"))
        .def_static("warning", &Debug::Warning, py::arg("message"))
        .def_static("error", &Debug::Error, py::arg("message"));
}
