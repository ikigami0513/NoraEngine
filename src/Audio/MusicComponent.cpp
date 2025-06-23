#include "Audio/MusicComponent.hpp"
#include "Utils/Debug.hpp"

MusicComponent::MusicComponent() {}
MusicComponent::~MusicComponent() {}

void MusicComponent::Start() {
    if (m_musicPath.empty()) return;

    m_music = LoadMusicStream(m_musicPath.c_str());

    if (m_music.ctxData == nullptr) {
        Debug::Error("Échec du chargement de la musique");
        return;
    }

    PlayMusicStream(m_music);
}

void MusicComponent::Update() {
    UpdateMusicStream(m_music);
}

std::string MusicComponent::GetMusicPath() const {
    return m_musicPath;
}

void MusicComponent::SetMusicPath(std::string musicPath) {
    m_musicPath = musicPath;
}