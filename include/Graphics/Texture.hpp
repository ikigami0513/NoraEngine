#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

class Texture {
    public:
        unsigned int ID;

        Texture(const std::string& path, bool hasAlpha = false, std::string type = "");
        void Bind(unsigned int unit = 0) const;
        void Unbind(unsigned int unit = 0) const;
        std::string Path();
        std::string Type();

    private:
        std::string m_path;
        std::string m_type;
};

#endif