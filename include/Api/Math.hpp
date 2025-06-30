#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace py = pybind11;

void bind_math(py::module_ m) {
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

    py::class_<glm::vec2>(m, "Vec2")
        .def(py::init<float, float>(), py::arg("x") = 0.0f, py::arg("y") = 0.0f)
        .def_readwrite("x", &glm::vec2::x)
        .def_readwrite("y", &glm::vec2::y)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self * float())
        .def(py::self / float())
        .def(float() * py::self)
        .def("normalize", [](glm::vec2 &v) {
            float len = glm::length(v);
            if (len > 0.0f) {
                v /= len;
            }
        }, "Normalize the vector in place")
        .def("normalized", [](const glm::vec2 &v) -> glm::vec2 {
            float len = glm::length(v);
            if (len > 0.0f) {
                return v / len;
            }
            return glm::vec2(0.0f, 0.0f);
        }, "Return a normalized copy of the vector")
        .def("__repr__", [](const glm::vec2& v) {
            return "<Vec2 x=" + std::to_string(v.x) + " y=" + std::to_string(v.y) + ">";
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
}
