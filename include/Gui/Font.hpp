#ifndef FONT_HPP
#define FONT_HPP

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

// Struct holding glyph rendering data
struct Character {
    unsigned int TextureID; // ID of the glyph texture
    glm::ivec2 Size; // Glyph size in pixels
    glm::ivec2 Bearing; // Offset from baseline to left/top
    unsigned int Advance; // Offset to advance to next glyph
};

// Font loader and glyph manager using FreeType
class Font {
    public:
        // Map of characters loaded
        std::map<char, Character> Characters;

        // Constructor: load font from file and initialize glyphs
        Font(const std::string& fontPath, unsigned int fontSize = 48);

        // Destructor: clean FreeType resources
        ~Font();

    private:
        FT_Face face = nullptr;
};

#endif