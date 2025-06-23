#ifndef SOUND_HPP
#define SOUND_HPP

#include "raudio.h"
#include <string>
#include "ECS/Component.hpp"

class SoundComponent : public Component {
    public:
        SoundComponent();
        ~SoundComponent();

        void Start() override;
        
        void InitSound();
        void Play();

        std::string GetSoundPath() const;
        void SetSoundPath(std::string soundPath);

    private:
        std::string m_soundPath;
        Sound m_sound;
};

#endif