#include "World/Scene.hpp"
#include "World/Component.hpp"
#include "Core/Window.hpp"

void Scene::AddEntity(std::shared_ptr<Entity> entity) {
    if (entity->GetParent() == nullptr) {
        m_rootEntities.push_back(entity);
    }
}

const std::vector<std::shared_ptr<Entity>>& Scene::GetRootEntities() const {
    return m_rootEntities;
}

void Scene::Start() {
    for (const auto& entity : m_rootEntities) {
        entity->Start();
    }
}

void Scene::Update() {
    for (const auto& entity : m_rootEntities) {
        entity->Update();
    }
}
