#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <stb_image/stb_image.h>

class Texture {
    public:
        unsigned int ID;

        Texture(const std::string& path, bool hasAlpha = false);
        void Bind(unsigned int unit = 0) const;
};

#endif