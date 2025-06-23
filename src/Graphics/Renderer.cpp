#include "Graphics/Renderer.hpp"

#include "World/Camera3D.hpp"
#include "World/Camera2D.hpp"
#include "Utils/Debug.hpp"
#include "Utils/Utils.hpp"
#include "Graphics/3D/Mesh/RenderComponent.hpp"
#include "Graphics/2D/Sprite.hpp"
#include "Core/AssetsManager.hpp"
#include "Gui/GuiComponent.hpp"
#include <unordered_map>

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
        // Save OpenGL state
        GLboolean wasDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
        GLboolean wasBlendEnabled = glIsEnabled(GL_BLEND);
        GLboolean wasCullFaceEnabled = glIsEnabled(GL_CULL_FACE);
        GLint oldBlendSrcAlpha, oldBlendDstAlpha, oldBlendEquationAlpha; // For more precise blend state restoration
        glGetIntegerv(GL_BLEND_SRC_ALPHA, &oldBlendSrcAlpha);
        glGetIntegerv(GL_BLEND_DST_ALPHA, &oldBlendDstAlpha);
        glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &oldBlendEquationAlpha);


        // Prepare for sprite rendering
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE); // Keeps culling enabled, ensure your quad winding is correct
        GL_CHECK_ERROR("Sprite render state setup");

        GL_CHECK_ERROR("Shader.Use");

        Camera2D* camera = cameraEntities[0]->GetComponent<Camera2D>();

        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix();
        std::vector<Entity*> spritedEntities = scene.GetEntitiesWithComponent<Sprite>();
        std::unordered_map<std::string, std::vector<Entity*>> entitiesByShader;

        for (Entity* entity : spritedEntities) {
            Sprite* sprite = entity->GetComponent<Sprite>();
            if (!sprite) continue;

            std::string shaderType = sprite->ShaderType();
            entitiesByShader[shaderType].push_back(entity);
        }

        for (const auto& pair : entitiesByShader) {
            const std::string& shaderType = pair.first;
            const std::vector<Entity*>& entities = pair.second;
            Shader* shader = AssetsManager::GetShader(shaderType);
            shader->Use();

            shader->SetMat4("view", view);
            shader->SetMat4("projection", projection);

            if (!shader) {
                Debug::Warning("Shader \"" + shaderType + "\" not found. Skipping group.");
                continue;
            }

            for (Entity* entity : entities) {
                Sprite* sprite = entity->GetComponent<Sprite>();
                if (!sprite) continue;
                sprite->Render(*shader);
            }
        }

        // Restore previous OpenGL state
        if (wasDepthTestEnabled) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);

        if (!wasBlendEnabled) glDisable(GL_BLEND);
        else {
            // Restore specific blend func if it was more complex or different
            glBlendFuncSeparate(oldBlendSrcAlpha, oldBlendDstAlpha, oldBlendSrcAlpha, oldBlendDstAlpha); // Or simply glBlendFunc
            // Could also restore GL_BLEND_EQUATION if changed
        }
        
        if (!wasCullFaceEnabled) glDisable(GL_CULL_FACE);
        GL_CHECK_ERROR("Restore OpenGL state");

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