#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "Api/Utils.hpp"
#include "Api/Math.hpp"
#include "Api/Graphics.hpp"
#include "Api/Core.hpp"
#include "Api/Input.hpp"
#include "Api/Audio.hpp"
#include "Api/Gui.hpp"
#include "Api/Physics.hpp"
#include "Api/Ecs.hpp"
#include "Api/PythonComponentWrapper.hpp"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(nora, m) {
    bind_utils(m);
    bind_math(m);
    bind_ecs(m);
    bind_graphics(m);
    bind_core(m);
    bind_input(m);
    bind_audio(m);
    bind_gui(m);
    bind_physics(m);
}
