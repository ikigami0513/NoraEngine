#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "Graphics/Color.hpp"
#include "Graphics/Texture.hpp"
#include "World/Camera2D.hpp"
#include "World/Camera3D.hpp"
#include "Graphics/3D/RenderComponent.hpp"
#include "Graphics/3D/Mesh/MeshedRenderComponent.hpp"
#include "Graphics/3D/Mesh/CuboidMesh.hpp"
#include "Graphics/3D/Mesh/SphereMesh.hpp"
#include "Graphics/3D/Mesh/CapsuleMesh.hpp"
#include "Graphics/3D/Mesh/3DModel/Model.hpp"
#include "Graphics/2D/Sprite.hpp"
#include "Graphics/2D/Animation2D.hpp"
#include "Graphics/3D/Skybox.hpp"

namespace py = pybind11;

void bind_graphics(py::module_ m) {
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

    py::class_<Texture, std::shared_ptr<Texture>>(m, "Texture")
        .def(py::init<const std::string&, bool>(), py::arg("path"), py::arg("flip_vertically") = false)
        .def_property_readonly("width", [](Texture& self) { return self.Width(); })
        .def_property_readonly("height", [](Texture& self) { return self.Height(); });

    py::class_<Camera3D, Component, std::shared_ptr<Camera3D>>(m, "Camera3D")
        .def(py::init<>())
        .def_property_readonly("front", [](const Camera3D& cam) {
            return cam.GetFront();
        }, "Direction vector the camera is facing.")
        .def_property_readonly("right", [](const Camera3D& cam) {
            return cam.GetRight();
        }, "Direction vector right of the camera.")
        .def_property("zoom", &Camera3D::GetZoom, &Camera3D::SetZoom, "Camera field of view in degrees.")
        .def_property("yaw", &Camera3D::GetYaw, &Camera3D::SetYaw, "Camera field of view in degrees.")
        .def_property("pitch", &Camera3D::GetPitch, &Camera3D::SetPitch, "Camera field of view in degrees.");

    py::class_<Camera2D, Component, std::shared_ptr<Camera2D>>(m, "Camera2D")
        .def(py::init<>())
        .def_property("target", &Camera2D::GetTarget, &Camera2D::SetTarget);

        py::class_<RenderComponent, Component, std::shared_ptr<RenderComponent>>(m, "RenderComponent");

        py::class_<Skybox, RenderComponent, std::shared_ptr<Skybox>>(m, "Skybox")
            .def(py::init<>())
            .def_property("faces", &Skybox::GetFaces, &Skybox::SetFaces);

        py::class_<MeshedRenderComponent, RenderComponent, std::shared_ptr<MeshedRenderComponent>>(m, "MeshedRenderComponent")
            .def_property("texture", &MeshedRenderComponent::GetTexture, &MeshedRenderComponent::SetTexture);

        py::class_<CuboidMesh, MeshedRenderComponent, std::shared_ptr<CuboidMesh>>(m, "CuboidMesh")
            .def(py::init<>());

        py::class_<SphereMesh, MeshedRenderComponent, std::shared_ptr<SphereMesh>>(m, "SphereMesh")
            .def(py::init<unsigned int, unsigned int>(), py::arg("sector_count") = 36, py::arg("stack_count") = 18);

        py::class_<CapsuleMesh, MeshedRenderComponent, std::shared_ptr<CapsuleMesh>>(m, "CapsuleMesh")
            .def(
                py::init<float, float, unsigned int, unsigned int, unsigned int>(),
                py::arg("radius") = 0.5f, py::arg("cylinder_height") = 1.0f, py::arg("sector_count") = 36,
                py::arg("hemisphere_stacks") = 18, py::arg("cylinder_stacks") = 10
            );

        py::class_<Model, MeshedRenderComponent, std::shared_ptr<Model>>(m, "Model")
            .def(py::init<std::string>(), py::arg("path") = "")
            .def_property("path", 
                [](Model& self) -> std::string {
                    return self.path;
                },
                [](Model& self, std::string new_path) {
                    self.path = new_path;
                }
            );

        py::class_<Sprite, Component, std::shared_ptr<Sprite>>(m, "Sprite")
            .def(py::init<>())
            .def_property("texture", &Sprite::GetTexture, &Sprite::SetTexture);

        py::class_<Animation2D, Component, std::shared_ptr<Animation2D>>(m, "Animation2D")
            .def(
                py::init<int, int, int, int, int, bool>(),
                py::arg("width"),
                py::arg("height"),
                py::arg("current_row"),
                py::arg("frames_count"),
                py::arg("animation_speed"),
                py::arg("repeat") = true
            )
            .def_property(
                "repeat",
                &Animation2D::GetRepeat,
                &Animation2D::SetRepeat
            )
            .def_property(
                "finish",
                &Animation2D::GetFinish,
                &Animation2D::SetFinish
            )
            .def_property(
                "current_frame",
                &Animation2D::GetCurrentFrame,
                &Animation2D::SetCurrentFrame
            )
            .def_property(
                "frames_count",
                &Animation2D::GetFramesCount,
                &Animation2D::SetFramesCount
            );
}
