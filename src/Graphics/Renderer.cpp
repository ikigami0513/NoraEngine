#include "Graphics/Renderer.hpp"

#include "World/Camera3D.hpp"
#include "World/Camera2D.hpp"
#include "Utils/Debug.hpp"
#include "Utils/Utils.hpp"
#include "Graphics/3D/RenderComponent.hpp"
#include "Core/AssetsManager.hpp"
#include "Gui/GuiComponent.hpp"
#include <algorithm>

Renderer::Renderer() {
    Init2DBatching();
}

Renderer::~Renderer() {
    Shutdown2DBatching();
}

void Renderer::Init2DBatching() {
    glGenVertexArrays(1, &m_spriteVAO);
    GL_CHECK_ERROR("Renderer::Init2DBatching - Gen VAO");

    glGenBuffers(1, &m_spriteVBO);
    GL_CHECK_ERROR("Renderer::Init2DBatching - Gen VBO");

    glBindVertexArray(m_spriteVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_spriteVBO);

    // Alloue l'espace pour le VBO, mais ne met pas encore de données.
    // GL_DYNAMIC_DRAW car les données changeront chaque frame.
    glBufferData(GL_ARRAY_BUFFER, MAX_BATCH_VERTICES * sizeof(SpriteVertex), nullptr, GL_DYNAMIC_DRAW);
    GL_CHECK_ERROR("Renderer::Init2DBatching - BufferData allocation");

    // Définition des attributs du sommet pour SpriteVertex
    // Position (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, position));
    glEnableVertexAttribArray(0);
    GL_CHECK_ERROR("Renderer::Init2DBatching - Position attribute");

    // Texture Coordinates (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, texCoords));
    glEnableVertexAttribArray(1);
    GL_CHECK_ERROR("Renderer::Init2DBatching - TexCoords attribute");

    // Color (location 2)
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, color));
    glEnableVertexAttribArray(2);
    GL_CHECK_ERROR("Renderer::Init2DBatching - Color attribute");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GL_CHECK_ERROR("Renderer::Init2DBatching - Unbind VAO/VBO final");

    // Pré-alloue l'espace pour le buffer CPU
    m_batchVerticesBuffer.reserve(MAX_BATCH_VERTICES);
}

void Renderer::Shutdown2DBatching() {
    if (m_spriteVAO != 0) {
        glDeleteVertexArrays(1, &m_spriteVAO);
        GL_CHECK_ERROR("Renderer::Shutdown2DBatching - delete VAO");
        m_spriteVAO = 0;
    }
    if (m_spriteVBO != 0) {
        glDeleteBuffers(1, &m_spriteVBO);
        GL_CHECK_ERROR("Renderer::Shutdown2DBatching - delete VBO");
        m_spriteVBO = 0;
    }
    if (m_spriteEBO != 0) {
        glDeleteBuffers(1, &m_spriteEBO);
        GL_CHECK_ERROR("Renderer::Shutdown2DBatching - delete EBO");
        m_spriteEBO = 0;
    }
}

void Renderer::AddSpriteToBatch(Sprite* sprite, Shader* shader, const glm::mat4& view, const glm::mat4& projection) {
    if (sprite == nullptr) return;

    std::shared_ptr<Texture> texture = sprite->GetTexture();

    // Si le VBO est plein, on vide le batch et on recommence
    if (m_numBatchedVertices + 6 > MAX_BATCH_VERTICES) {
        FlushBatch(shader, view, projection);
    }

    // Récupère les données des sommets du sprite, déjà transformées et colorées
    std::array<SpriteVertex, 6> spriteVertices = sprite->GetVerticesData();

    // Ajoute les sommets au buffer temporaire CPU
    for (int i = 0; i < 6; i++) {
        m_batchVerticesBuffer.push_back(spriteVertices[i]);
    }
    m_numBatchedVertices += 6;
}

void Renderer::FlushBatch(Shader* shader, const glm::mat4& view, const glm::mat4& projection) {
    if (m_numBatchedVertices == 0) return; // Rien à dessiner

    // Active le shader
    shader->Use();
    shader->SetMat4("view", view);
    shader->SetMat4("projection", projection);
    shader->SetInt("image", 0); // Active la texture slot 0

    // Bind et envoie les données au VBO
    glBindVertexArray(m_spriteVAO);
    GL_CHECK_ERROR("FlushBatch - After VAO Bind");

    glBindBuffer(GL_ARRAY_BUFFER, m_spriteVBO);
    GL_CHECK_ERROR("FlushBatch - After VBO Bind");
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_numBatchedVertices * sizeof(SpriteVertex), m_batchVerticesBuffer.data());
    GL_CHECK_ERROR("FlushBatch - glBufferSubData");

    // Dessine tous les sprites dans le batch
    glDrawArrays(GL_TRIANGLES, 0, m_numBatchedVertices);
    GL_CHECK_ERROR("FlushBatch - glDrawArrays");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GL_CHECK_ERROR("FlushBatch - Unbind VAO/VBO");

    // Réinitialise le buffer pour le prochain batch
    m_batchVerticesBuffer.clear();
    m_numBatchedVertices = 0;
    m_texturesInBatch.clear();
}

bool Renderer::Rendering2D(Scene scene, int width, int height) {
    std::vector<Entity*> cameraEnties = scene.GetEntitiesWithComponent<Camera2D>();
    if (!cameraEnties.empty()) {
        Camera2D* camera = cameraEnties[0]->GetComponent<Camera2D>();
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix();
        AABB viewAABB = camera->GetViewAABB();

        std::vector<Entity*> spritedEntities = scene.GetEntitiesWithComponent<Sprite>();

        // Organiser les sprites par shader et puis par texture
        // Pour un batching simple (un shader, une texture par batch), on va vider le batch
        // chaque fois qu'on change de texture.
        // Si vous avez un shader qui gère un tableau de samplers (texture array),
        // alors la logique ici sera différente et vous pourrez regrouper plus de sprites.

        std::map<std::string, std::map<std::string, std::vector<Sprite*>>> sortedSprites;

        for (Entity* entity : spritedEntities) {
            Sprite* sprite = entity->GetComponent<Sprite>();
            if (!sprite) continue;

            AABB spriteAABB = sprite->GetWorldAABB();
            if (!viewAABB.Intersects(spriteAABB)) continue; // Frustum culling

            std::string shaderType = sprite->ShaderType();
            std::string textureKey = "no_texture";
            if (sprite->GetTexture()) {
                textureKey = sprite->GetTexture()->Path();
            }
            sortedSprites[shaderType][textureKey].push_back(sprite);
        }

        // Itérer sur les groupes de shaders
        GL_CHECK_ERROR("Before Batch Rendering Loop");
        for (auto const& [shaderType, textureGroups] : sortedSprites) {
            Shader* currentShader = AssetsManager::GetShader(shaderType);
            if (!currentShader) {
                Debug::Warning("Shader \"" + shaderType + "\" not found. Skipping group.");
                continue;
            }
            currentShader->Use();

            // Itérer sur les groupes de textures pour ce shader
            for (auto const& [texturePath, spritesToRender] : textureGroups) {
                // Si la texture est présente, bindez-la. Sinon, le shader doit gérer le cas sans texture (couleur unie).
                if (texturePath != "no_texture") {
                    std::shared_ptr<Texture> texture = spritesToRender[0]->GetTexture();
                    if (texture) {
                        texture->Bind(0);
                        currentShader->SetInt("useTexture", 1);
                        currentShader->SetInt("image", 0);
                    }
                    else {
                        Debug::Warning("Texture '" + texturePath + "' not found for sprite rendering.");
                        currentShader->SetInt("useTexture", 0);
                    }
                }
                else {
                    currentShader->SetInt("useTexture", 0);
                }

                for (Sprite* sprite : spritesToRender) {
                    AddSpriteToBatch(sprite, currentShader, view, projection);
                }

                FlushBatch(currentShader, view, projection);

                // Unbind la texture après le batch
                if (texturePath != "no_texture") {
                    std::shared_ptr<Texture> texture = spritesToRender[0]->GetTexture();
                    if (texture) {
                        texture->Unbind(0);
                    }
                }
            }
        }

        return true;
    }

    return false;
}

bool Renderer::Rendering3D(Scene scene, int width, int height) {
    std::vector<Entity*> cameraEntities = scene.GetEntitiesWithComponent<Camera3D>();
    if (!cameraEntities.empty()) {
        Camera3D* camera = cameraEntities[0]->GetComponent<Camera3D>();

        if (camera->hasFog) {
            Debug::Info("camera has Fog");
        }

        // camera/view transformation
        glm::mat4 view = camera->GetViewMatrix();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), (float)width / (float)height, 0.1f, 100.0f);
        std::vector<Entity*> meshedEntities = scene.GetEntitiesWithComponent<RenderComponent>();
        for (Entity* entity : meshedEntities) {
            RenderComponent* mesh = entity->GetComponent<RenderComponent>();
            Shader* shader = AssetsManager::GetShader(mesh->ShaderType());
            shader->Use();
            if (shader->HasUniform("hasFog") && camera->hasFog) {
                shader->SetBool("hasFog", true);
                shader->SetVec4("fogColor", glm::vec4(camera->fogColor.r, camera->fogColor.g, camera->fogColor.b, camera->fogColor.alpha));
                shader->SetFloat("minFogDist", camera->minFogDist);
                shader->SetFloat("maxFogDist", camera->maxFogDist);
                GL_CHECK_ERROR("Activate Fog");
            }
            else {
                shader->SetBool("hasFog", false);
            }
            mesh->Render(*shader, view, projection);
        }

        return true;
    }

    return false;
}

void Renderer::RenderingGUI(Scene scene, int width, int height) {
    // render gui
    glm::mat4 ortho_projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);

    std::vector<Entity*> guiEntities = scene.GetEntitiesWithComponent<GuiComponent>();
    for (auto entity : guiEntities) {
        std::vector<GuiComponent*> guis = entity->GetComponents<GuiComponent>();
        for (const auto& gui : guis) {
            Shader* shader = AssetsManager::GetShader(gui->ShaderType());
            shader->Use();
            shader->SetMat4("projection", ortho_projection);
            gui->Render(*shader);
        }
    }
}