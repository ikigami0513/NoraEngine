#ifndef SCENE_HPP
#define SCENE_HPP

#include "World/Entity.hpp"
#include <vector>
#include <memory>
#include <functional>

class Scene {
    private:
        std::vector<std::shared_ptr<Entity>> m_rootEntities;

    public:
        Scene() = default;
        ~Scene() = default;

        void AddEntity(std::shared_ptr<Entity> entity);
        const std::vector<std::shared_ptr<Entity>>& GetRootEntities() const;

        void Start();
        void Update();
};

#endif