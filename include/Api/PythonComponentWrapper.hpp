#ifndef PYTHON_COMPONENT_WRAPPER_HPP
#define PYTHON_COMPONENT_WRAPPER_HPP

#include "World/Component.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

class PythonComponentWrapper : public Component {
    public:
        PythonComponentWrapper(py::object py_component);

        void Start() override;
        void Update() override;

        py::object PyComponent() const;

    private:
        py::object py_component_;
};

#endif