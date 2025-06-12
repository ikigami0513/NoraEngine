#include "Api/PythonComponentWrapper.hpp"
#include <iostream>
#include "World/Entity.hpp"

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
            py::function update_func = py_component_.attr("update");
            update_func();
        }
    }
    catch (const py::error_already_set& e) {
        std::cerr << "Python exception in PythonComponentWrapper::Update: " << e.what() << std::endl;
    }
}

void PythonComponentWrapper::OnCollisionEnter(Entity* other) {
    try {
        if (py::hasattr(py_component_, "on_collision_enter")) {
            py::function on_collision_enter_func = py_component_.attr("on_collision_enter");
            on_collision_enter_func(other);
        }
    }
    catch (const py::error_already_set& e) {
        std::cerr << "Python exception in PythonComponentWrapper::OnCollisionEnter " << e.what() << std::endl;
    }
}

py::object PythonComponentWrapper::PyComponent() const {
    return py_component_;
}
