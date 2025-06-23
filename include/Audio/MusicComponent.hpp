#ifndef MUSIC_COMPONENT_HPP
#define MUSIC_COMPONENT_HPP

#include "raudio.h"
#include <string>
#include "ECS/Component.hpp"

class MusicComponent : public Component {
    public:
        MusicComponent();
        ~MusicComponent();

        void Start() override;
        void Update() override;
        void Play();

        std::string GetMusicPath() const;
        void SetMusicPath(std::string musicPath);

    private:
        std::string m_musicPath;
        Music m_music;
};

#endif
