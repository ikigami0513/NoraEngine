#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "ECS/Component.hpp"
#include "Graphics/Texture.hpp"
#include "glm/glm.hpp"
#include <vector>

class Tileset {
    public:
        Tileset(std::string p_name, int p_firstGid, std::string p_source) {
            name = p_name;
            firstGid = p_firstGid;
            source = p_source;

            m_texture = new Texture(source);
        }

        std::string name;
        int firstGid;
        std::string source;
        Texture* GetTexture() {
            return m_texture;
        }

    private:
        Texture* m_texture;
}; 

struct Layer {
    std::string name;
    int width;
    int height;
    std::vector<int> tiles;
};

class Tilemap : public Component {
    public:
        Tilemap();
        ~Tilemap();

        void Start() override;

        void LoadTilemapFromFile();

        std::string GetTilemapPath() const;
        void SetTilemapPath(const std::string& path);

        Tileset GetTextureFromGid(const int gid);
        glm::vec2 GetTileTexCoordFromGid(int gid, Tileset t);

    private:
        std::string m_tilemapPath;
        int m_width;
        int m_height;
        int m_tileWidth;
        int m_tileHeight;
        std::vector<Tileset> m_tilesets;
        std::vector<Layer> m_layers;
};

#endif