#ifndef ASSETS_MANAGER_HPP
#define ASSETS_MANAGER_HPP

#include <map>
#include <memory>

#include "Graphics/Shader.hpp"

class AssetsManager {
    public:
        static void AddShader(std::string name, std::unique_ptr<Shader> shader);

        static Shader* GetShader(const std::string& name);

    private:
        static std::map<std::string, std::unique_ptr<Shader>> m_shaders;
};

#endif