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

        template<typename T> 
        std::vector<Entity*> GetEntitiesWithComponent() const {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

            std::vector<Entity*> result;

            std::function<void(Entity*)> search = [&](Entity* entity) {
                if (entity->GetComponent<T>() != nullptr) {
                    result.push_back(entity);
                }

                for (const auto& child : entity->GetChildren()) {
                    search(child.get());
                }
            };

            for (const auto& root : m_rootEntities) {
                search(root.get());
            }

            return result;
        }
};

#endif