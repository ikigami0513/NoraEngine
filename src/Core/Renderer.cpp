#include "Core/Renderer.hpp"

#include "World/Camera3D.hpp"
#include "World/Camera2D.hpp"
#include "World/Mesh/RenderComponent.hpp"
#include "Graphics/Sprite.hpp"
#include "Core/AssetsManager.hpp"
#include "Gui/GuiComponent.hpp"

bool Renderer::Rendering3D(Scene scene, int width, int height) {
    std::vector<Entity*> cameraEntities = scene.GetEntitiesWithComponent<Camera3D>();
    if (!cameraEntities.empty()) {
        Camera3D* camera = cameraEntities[0]->GetComponent<Camera3D>();

        // camera/view transformation
        glm::mat4 view = camera->GetViewMatrix();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), (float)width / (float)height, 0.1f, 100.0f);
        std::vector<Entity*> meshedEntities = scene.GetEntitiesWithComponent<RenderComponent>();
        for (Entity* entity : meshedEntities) {
            RenderComponent* mesh = entity->GetComponent<RenderComponent>();
            Shader* shader = AssetsManager::GetShader(mesh->ShaderType());
            mesh->Render(*shader, view, projection);
        }

        return true;
    }

    return false;
}

bool Renderer::Rendering2D(Scene scene, int width, int height) {
    std::vector<Entity*> cameraEntities = scene.GetEntitiesWithComponent<Camera2D>();
    if (!cameraEntities.empty()) {
        Camera2D* camera = cameraEntities[0]->GetComponent<Camera2D>();

        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix();
        std::vector<Entity*> spritedEntities = scene.GetEntitiesWithComponent<Sprite>();
        for (Entity* entity : spritedEntities) {
            Sprite* sprite = entity->GetComponent<Sprite>();
            Shader* shader = AssetsManager::GetShader(sprite->ShaderType());
            sprite->Render(*shader, view, projection);
        }

        return true;
    }

    return false;
}

void Renderer::RenderingGUI(Scene scene, int width, int height) {
    // render gui
    glm::mat4 ortho_projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
    Shader* guiShader = AssetsManager::GetShader("gui");
    guiShader->Use();
    guiShader->SetMat4("projection", ortho_projection);

    std::vector<Entity*> guiEntities = scene.GetEntitiesWithComponent<GuiComponent>();
    for (auto entity : guiEntities) {
        GuiComponent* gui = entity->GetComponent<GuiComponent>();
        Shader* shader = AssetsManager::GetShader(gui->ShaderType());
        gui->Render(*shader);
    }
}