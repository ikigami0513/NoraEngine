#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "Audio/SoundComponent.hpp"
#include "Audio/MusicComponent.hpp"

namespace py = pybind11;

void bind_audio(py::module_ m) {
    py::class_<SoundComponent, Component, std::shared_ptr<SoundComponent>>(m, "SoundComponent")
        .def(py::init<>())
        .def_property("sound_path", &SoundComponent::GetSoundPath, &SoundComponent::SetSoundPath)
        .def("play", &SoundComponent::Play);

    py::class_<MusicComponent, Component, std::shared_ptr<MusicComponent>>(m, "MusicComponent")
        .def(py::init<>())
        .def_property("music_path", &MusicComponent::GetMusicPath, &MusicComponent::SetMusicPath);
}
