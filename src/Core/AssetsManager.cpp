#include "Core/AssetsManager.hpp"

std::map<std::string, std::unique_ptr<Shader>> AssetsManager::m_shaders = {};

void AssetsManager::AddShader(std::string name, std::unique_ptr<Shader> shader) {
    m_shaders.insert({name, std::move(shader)});
}

Shader* AssetsManager::GetShader(const std::string& name) {
    auto it = m_shaders.find(name);
    if (it != m_shaders.end()) {
        return it->second.get();
    }
    return nullptr;
}