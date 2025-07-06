#include "Graphics/2D/Tilemap.hpp"
#include "Utils/Debug.hpp"
#include "ECS/Entity.hpp"
#include "Graphics/2D/Sprite.hpp"
#include "Core/Window.hpp"
#include <Utils/tinyxml2.h>
#include <sstream>

Tilemap::Tilemap() {}

Tilemap::~Tilemap() {}

void Tilemap::Start() {
    if (m_tilemapPath.empty()) {
        Debug::Error("Tilemap path is empty.");
        return;
    }

    LoadTilemapFromFile();
}

void Tilemap::LoadTilemapFromFile() {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(m_tilemapPath.c_str()) != tinyxml2::XML_SUCCESS) {
        Debug::Error("Failed to load tilemap: " + m_tilemapPath);
        return;
    }

    // <tilemap>
    tinyxml2::XMLElement* root = doc.FirstChildElement("tilemap");
    m_width = root->IntAttribute("width");
    m_height = root->IntAttribute("height");
    m_tileWidth = root->IntAttribute("tilewidth");
    m_tileHeight = root->IntAttribute("tileheight");

    // <tilesets>
    tinyxml2::XMLElement* tilesetsElement = root->FirstChildElement("tilesets");
    for (tinyxml2::XMLElement* tileset = tilesetsElement->FirstChildElement("tileset"); tileset; tileset=tileset->NextSiblingElement("tileset")) {
        Tileset ts(tileset->Attribute("name"), tileset->IntAttribute("firstgid"), tileset->Attribute("source"));
        ts.name = tileset->Attribute("name");
        ts.firstGid = tileset->IntAttribute("firstgid");
        ts.source = tileset->Attribute("source");
        m_tilesets.push_back(ts);
    }

    // <layers>
    tinyxml2::XMLElement* layersElement = root->FirstChildElement("layers");
    for (tinyxml2::XMLElement* layer = layersElement->FirstChildElement("layer"); layer; layer = layer->NextSiblingElement("layer")) {
        Layer l;
        l.name = layer->Attribute("name");
        l.width = layer->IntAttribute("width");
        l.height = layer->IntAttribute("height");

        tinyxml2::XMLElement* data = layer->FirstChildElement("data");
        const char* csv = data->GetText();
        std::stringstream ss(csv);
        std::string item;

        int index = 0;
        while (std::getline(ss, item, ',')) {
            int gid = std::stoi(item);
            l.tiles.push_back(gid);

            if (gid == 0) {
                index++;
                continue;
            }

            std::shared_ptr<Entity> tile = std::make_shared<Entity>();
            std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();
            Tileset tileset = GetTextureFromGid(gid);
            if (!tileset.GetTexture()) {
                Debug::Error("Tileset has not Texture.");
            }
            sprite->SetTexture(std::make_unique<Texture>(*tileset.GetTexture()));
            glm::vec2 texCoords = GetTileTexCoordFromGid(gid, tileset);
            sprite->SetTextureRect(texCoords.x, texCoords.y, m_tileWidth, m_tileHeight);

            int tileX = index % l.width;
            int tileY = index / l.width;

            float worldX = static_cast<float>(tileX * m_tileWidth);
            float worldY = static_cast<float>(tileY * m_tileHeight);

            std::cout << "X: " << worldX << " Y: " << worldY << std::endl;

            tile->GetTransform().SetLocalPosition({worldX, worldY, m_owner->GetTransform().GetLocalPosition().z});

            sprite->SetOwner(tile.get());
            tile->AddComponent(sprite);
            Window::GetInstance().GetScene().AddEntity(tile);
            index++;
        }

        m_layers.push_back(l);
    }
}

std::string Tilemap::GetTilemapPath() const {
    return m_tilemapPath;
}

void Tilemap::SetTilemapPath(const std::string& path) {
    m_tilemapPath = path;
}

Tileset Tilemap::GetTextureFromGid(const int gid) {
    for (int i = 0; i < m_tilesets.size(); i++) {
        if (i + 1 < m_tilesets.size()) {
            if (m_tilesets[i].firstGid <= gid && gid < m_tilesets[i + 1].firstGid) {
                return m_tilesets[i];
            }
        }
        else if (gid >= m_tilesets[i].firstGid) {
            return m_tilesets[i];
        }
    }
}

glm::vec2 Tilemap::GetTileTexCoordFromGid(int gid, Tileset t) {
    int localId = gid - t.firstGid;

    int tilesPerRow = t.GetTexture()->Width() / m_tileWidth;
    int x = localId % tilesPerRow;
    int y = localId / tilesPerRow;

    return {
        static_cast<float>(x * m_tileWidth),
        static_cast<float>(y * m_tileHeight)
    };
}
