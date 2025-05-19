#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include "Core/Time.hpp"
#include "Core/Input.hpp"
#include "Core/Key.hpp"
#include "Graphics/Color.hpp"
#include "World/Entity.hpp"
#include "World/Component.hpp"
#include "World/Camera.hpp"
#include "World/Transform.hpp"
#include "World/Mesh/CuboidMesh.hpp"
#include "World/Mesh/SphereMesh.hpp"
#include "World/Mesh/CapsuleMesh.hpp"
#include "Gui/Font.hpp"
#include "Gui/GuiComponent.hpp"
#include "Gui/Text.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Api/PythonComponentWrapper.hpp"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(nora, m) {
    py::class_<glm::vec3>(m, "Vec3")
        .def(py::init<float, float, float>(), py::arg("x") = 0.0f, py::arg("y") = 0.0f, py::arg("z") = 0.0f)
        .def_readwrite("x", &glm::vec3::x)
        .def_readwrite("y", &glm::vec3::y)
        .def_readwrite("z", &glm::vec3::z)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self * float())
        .def(py::self / float())
        .def(float() * py::self)
        .def("__repr__", [](const glm::vec3& v) {
            return "<Vec3 x=" + std::to_string(v.x) + " y=" + std::to_string(v.y) + " z=" + std::to_string(v.z) + ">";
        });

    py::class_<glm::mat4>(m, "Mat4")
        .def(py::init<>())
        .def("__repr__", [](const glm::mat4& mat) {
            std::string s = "<Mat4\n";
            for (int i = 0; i < 4; ++i) {
                s += "[";
                for (int j = 0; j < 4; ++j) {
                    s += std::to_string(mat[i][j]);
                    if (j < 3) s += ", ";
                }
                s += "]\n";
            }
            s += ">";
            return s;
        });

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

    py::enum_<Key>(m, "Key")
        .value("Unknown", Key::Unknown)
        .value("Space", Key::Space)
        .value("Apostrophe", Key::Apostrophe)
        .value("Comma", Key::Comma)
        .value("Minus", Key::Minus)
        .value("Period", Key::Period)
        .value("Slash", Key::Slash)
        .value("Num0", Key::Num0)
        .value("Num1", Key::Num1)
        .value("Num2", Key::Num2)
        .value("Num3", Key::Num3)
        .value("Num4", Key::Num4)
        .value("Num5", Key::Num5)
        .value("Num6", Key::Num6)
        .value("Num7", Key::Num7)
        .value("Num8", Key::Num8)
        .value("Num9", Key::Num9)
        .value("A", Key::A)
        .value("B", Key::B)
        .value("C", Key::C)
        .value("D", Key::D)
        .value("E", Key::E)
        .value("F", Key::F)
        .value("G", Key::G)
        .value("H", Key::H)
        .value("I", Key::I)
        .value("J", Key::J)
        .value("K", Key::K)
        .value("L", Key::L)
        .value("M", Key::M)
        .value("N", Key::N)
        .value("O", Key::O)
        .value("P", Key::P)
        .value("Q", Key::Q)
        .value("R", Key::R)
        .value("S", Key::S)
        .value("T", Key::T)
        .value("U", Key::U)
        .value("V", Key::V)
        .value("W", Key::W)
        .value("X", Key::X)
        .value("Y", Key::Y)
        .value("Z", Key::Z)
        .value("Escape", Key::Escape)
        .value("Enter", Key::Enter)
        .value("Tab", Key::Tab)
        .value("Backspace", Key::Backspace)
        .value("Insert", Key::Insert)
        .value("Delete", Key::Delete)
        .value("Right", Key::Right)
        .value("Left", Key::Left)
        .value("Down", Key::Down)
        .value("Up", Key::Up)
        .export_values();

    py::enum_<MouseCode>(m, "MouseCode")
        .value("Button1", MouseCode::Button1)
        .value("Button2", MouseCode::Button2)
        .value("Button3", MouseCode::Button3)
        .value("Button4", MouseCode::Button4)
        .value("Button5", MouseCode::Button5)
        .value("Button6", MouseCode::Button6)
        .value("Button7", MouseCode::Button7)
        .value("Button8", MouseCode::Button8)
        .value("Left", MouseCode::Left)
        .value("Right", MouseCode::Right)
        .value("Middle", MouseCode::Middle)
        .export_values();

    py::class_<Input>(m, "Input")
        .def_static("is_key_pressed", &Input::IsKeyPressed, py::arg("key"), "Returns True if the specified key is currently pressed.")
        .def_static("is_just_pressed", &Input::IsJustPressed, py::arg("key"), "Returns True if the specified key is just pressed.")
        .def_static("is_just_released", &Input::IsJustReleased, py::arg("key"), "Returns True if the specified key is just released.")
        .def_static("is_mouse_button_pressed", &Input::IsMouseButtonPressed, py::arg("mouse_code"), "Returns True if the specified mouse button is currently pressed.")
        .def_static("is_mouse_button_just_pressed", &Input::IsMouseButtonJustPressed, py::arg("mouse_code"), "Returns True if the specified mouse button is just pressed.")
        .def_static("is_mouse_button_just_released", &Input::IsMouseButtonJustReleased, py::arg("mouse_code"), "Returns True if the specified mouse button is just released.")
        .def_property_readonly_static(
            "mouse_position",
            [](py::object) {
                return Input::GetMousePosition();
            },
            "Returns current mouse position."
        )
        .def_property_readonly_static(
            "mouse_delta",
            [](py::object) {
                return Input::GetMouseDelta();
            },
            "Returns current mouse delta."
        )
        .def_property_readonly_static(
            "scroll_delta",
            [](py::object) {
                return Input::GetScrollDelta();
            },
            "Returns current mouse scroll delta."
        );

    py::class_<Component, std::shared_ptr<Component>>(m, "Component")
        .def(py::init<>())
        .def("start", &Component::Start)
        .def("update", &Component::Update)
        .def("set_owner", &Component::SetOwner)
        .def_property_readonly("owner", [](const Component& self) {
            return self.GetOwner();
        });

    py::class_<Camera, Component, std::shared_ptr<Camera>>(m, "Camera")
        .def(py::init<>())
        .def_property_readonly("front", [](const Camera& cam) {
            return cam.GetFront();
        }, "Direction vector the camera is facing.")
        .def_property_readonly("right", [](const Camera& cam) {
            return cam.GetRight();
        }, "Direction vector right of the camera.")
        .def_property("zoom", &Camera::GetZoom, &Camera::SetZoom, "Camera field of view in degrees.")
        .def_property("yaw", &Camera::GetYaw, &Camera::SetYaw, "Camera field of view in degrees.")
        .def_property("pitch", &Camera::GetPitch, &Camera::SetPitch, "Camera field of view in degrees.");

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

        py::class_<Texture, std::shared_ptr<Texture>>(m, "Texture")
            .def(py::init<const std::string&, bool>(), py::arg("path"), py::arg("flip_vertically") = false);

        py::class_<Font, std::shared_ptr<Font>>(m, "Font")
            .def(py::init<const std::string&, unsigned int>(), py::arg("font_path"), py::arg("font_size") = 48);

        py::class_<RenderComponent, Component, std::shared_ptr<RenderComponent>>(m, "RenderComponent")
            .def_property("texture", &RenderComponent::GetTexture, &RenderComponent::SetTexture);

        py::class_<CuboidMesh, RenderComponent, std::shared_ptr<CuboidMesh>>(m, "CuboidMesh")
            .def(py::init<>());

        py::class_<SphereMesh, RenderComponent, std::shared_ptr<SphereMesh>>(m, "SphereMesh")
            .def(py::init<unsigned int, unsigned int>(), py::arg("sector_count") = 36, py::arg("stack_count") = 18);

        py::class_<CapsuleMesh, RenderComponent, std::shared_ptr<CapsuleMesh>>(m, "CapsuleMesh")
            .def(
                py::init<float, float, unsigned int, unsigned int, unsigned int>(),
                py::arg("radius") = 0.5f, py::arg("cylinder_height") = 1.0f, py::arg("sector_count") = 36,
                py::arg("hemisphere_stacks") = 18, py::arg("cylinder_stacks") = 10
            );

        py::class_<GuiComponent, Component, std::shared_ptr<GuiComponent>>(m, "GuiComponent");

        py::class_<Text, GuiComponent, std::shared_ptr<Text>>(m, "Text")
            .def(py::init<>())
            .def_property("font",
                [](Text& self) -> std::shared_ptr<Font> { // Getter
                    return self.font; // Retourne directement le shared_ptr
                },
                [](Text& self, std::shared_ptr<Font> new_font) { // Setter
                    self.font = new_font; // Assignation directe du shared_ptr
                },
                "The font used by the text component.")
            .def_property("text",
                [](const Text& self) { return self.text; },
                [](Text& self, const std::string& new_text) { self.text = new_text; },
                "The content of the text.")
            .def_property("color",
                [](const Text& self) { return self.color; },
                [](Text& self, const Color& new_color) { self.color = new_color; },
                "The color of the text.");
}
