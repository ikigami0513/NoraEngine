#include "Audio/SoundComponent.hpp"
#include "Utils/Debug.hpp"

SoundComponent::SoundComponent() {}
SoundComponent::~SoundComponent() {}

void SoundComponent::Start() {
    InitSound();
}

void SoundComponent::InitSound() {
    m_sound = LoadSound(m_soundPath.c_str());
}

void SoundComponent::Play() {
    if (m_sound.sampleCount == 0) {
        Debug::Error("Aucun son n'a été lié.");
        return;
    }
    PlaySound(m_sound);
}

std::string SoundComponent::GetSoundPath() const {
    return m_soundPath;
}

void SoundComponent::SetSoundPath(std::string soundPath) {
    m_soundPath = soundPath;
    InitSound();
}
