#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "ECS/Scene.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/2D/Sprite.hpp"
#include <vector>
#include <glad/glad.h>

class Renderer {
    public:
        Renderer();
        ~Renderer();

        void Init2DBatching();
        void Shutdown2DBatching();

        bool Rendering3D(Scene scene, int width, int height);
        bool Rendering2D(Scene scene, int width, int height);
        void RenderingGUI(Scene scene, int width, int height);

    private:
        GLuint m_spriteVAO = 0;
        GLuint m_spriteVBO = 0;
        GLuint m_spriteEBO = 0;

        // Taille maximale du buffer pour les sommets des sprites
        static const GLsizei MAX_BATCH_VERTICES = 60000; // 10_000 sprites * 6 vertices/sprite
        static const GLsizei MAX_BATCH_INDICES = 30000;  // 10_000 sprites * 6 indices/sprite

        // Buffer temporaire pour collecter les données des sommets avant l'upload au VBO
        std::vector<SpriteVertex> m_batchVerticesBuffer;

        // Garde une trace du nombre actuel de sommets dans le buffer
        int m_numBatchedVertices = 0;

        // Garde une trace des textures utilisées et leur slot actif
        std::vector<std::shared_ptr<Texture>> m_texturesInBatch;
        static const int MAX_TEXTURE_SLOTS = 32; // Limite du GPU pour les textures bindées simultanément

        // Fonction pour vider le batch et le dessiner
        void FlushBatch(Shader* shader, const glm::mat4& view, const glm::mat4& projection);

        // Fonction pour préparer le dessin d'un sprite (ajouter au batch)
        void AddSpriteToBatch(Sprite* sprite, Shader* shader, const glm::mat4& view, const glm::mat4& projection);

        // Ajout d'un cache pour les shaders 2D
        std::unordered_map<std::string, Shader*> m_cached2DShaders;
};

#endif