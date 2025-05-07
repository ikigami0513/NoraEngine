#include "Api/PythonComponentWrapper.hpp"
#include <iostream>

PythonComponentWrapper::PythonComponentWrapper(py::object py_component) : py_component_(py_component) {}

void PythonComponentWrapper::Start() {
    try {
        if (py::hasattr(py_component_, "start")) {
            py::function start_func = py_component_.attr("start");
            start_func();
        }
    }
    catch (const py::error_already_set& e) {
        std::cerr << "Python exception in PythonComponentWrapper::Start: " << e.what() << std::endl;
    }
}

void PythonComponentWrapper::Update() {
    try {
        if (py::hasattr(py_component_, "update")) {
            py::function start_func = py_component_.attr("update");
            start_func();
        }
    }
    catch (const py::error_already_set& e) {
        std::cerr << "Python exception in PythonComponentWrapper::Update: " << e.what() << std::endl;
    }
}

py::object PythonComponentWrapper::PyComponent() const {
    return py_component_;
}
