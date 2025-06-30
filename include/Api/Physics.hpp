#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "Physics/Transform.hpp"
#include "Physics/2D/RectCollider.hpp"
#include "Physics/2D/Rigidbody2D.hpp"

namespace py = pybind11;

void bind_physics(py::module_ m) {
    py::class_<Transform>(m, "Transform")
        .def(py::init<>())
        .def_property("local_position", &Transform::GetLocalPosition, &Transform::SetLocalPosition)
        .def_property("local_rotation", &Transform::GetLocalRotation, &Transform::SetLocalRotation)
        .def_property("local_scale", &Transform::GetLocalScale, &Transform::SetLocalScale)
        .def_property_readonly("global_position", &Transform::GetGlobalPosition)
        .def_property_readonly("model_matrix", &Transform::GetModelMatrix)
        .def_property_readonly("right", &Transform::GetRight)
        .def_property_readonly("up", &Transform::GetUp)
        .def_property_readonly("backward", &Transform::GetBackward)
        .def_property_readonly("forward", &Transform::GetForward)
        .def_property_readonly("global_scale", &Transform::GetGlobalScale)
        .def_property_readonly("is_dirty", &Transform::IsDirty);

    py::class_<RectCollider, Component, std::shared_ptr<RectCollider>>(m, "RectCollider")
        .def(py::init<>())
        .def_property("offset", [](const RectCollider& collider) { return collider.offset; }, [](RectCollider& collider, const Offset& p_offset) { collider.offset = p_offset; });

    py::class_<Rigidbody2D, Component, std::shared_ptr<Rigidbody2D>>(m, "Rigidbody2D")
        .def(py::init<>())
        .def_property(
            "velocity",
            &Rigidbody2D::GetVelocity,
            &Rigidbody2D::SetVelocity
        );
}
