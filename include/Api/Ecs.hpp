#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "ECS/Component.hpp"
#include "ECS/Scene.hpp"

namespace py = pybind11;

void bind_ecs(py::module_ m) {
    py::class_<Component, std::shared_ptr<Component>>(m, "Component")
        .def(py::init<>())
        .def("start", &Component::Start)
        .def("update", &Component::Update)
        .def("on_collision_enter", &Component::OnCollisionEnter)
        .def("set_owner", &Component::SetOwner)
        .def_property_readonly("owner", [](const Component& self) {
            return self.GetOwner();
        });

    py::class_<Scene>(m, "Scene")
        .def(py::init<>())
        .def("add_entity", &Scene::AddEntity)
        .def("get_root_entities", &Scene::GetRootEntities);

    py::class_<Entity, std::shared_ptr<Entity>>(m, "Entity")
        .def(py::init<>())
        .def("add_component", [](Entity& self, const py::object& py_comp) {
            auto comp = std::make_shared<PythonComponentWrapper>(py_comp);
            comp->SetOwner(&self);
            self.AddComponent(comp);
        })
        .def("get_component", [](const Entity& self, const py::object& type) -> py::object {
            for (const auto& comp : self.Components()) {
                if (auto py_wrapper = dynamic_cast<PythonComponentWrapper*>(comp.get())) {
                    if (py::isinstance(py_wrapper->PyComponent(), type)) {
                        return py_wrapper->PyComponent();
                    }
                }
            }
            return py::none();
        }, py::return_value_policy::reference)
        .def_property_readonly("transform", [](const Entity& self) -> const Transform& {
            return self.GetTransform();
        });
}
