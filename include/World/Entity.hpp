#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "World/Transform.hpp"
#include "World/Component.hpp"

#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <typeinfo>
#include <iostream>

class Entity {
    private:
        Transform m_transform;
        Entity* m_parent = nullptr;

        std::vector<std::shared_ptr<Component>> m_components;
        std::vector<std::unique_ptr<Entity>> m_children;

    public:
        Entity() = default;
        ~Entity() = default;

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;

        const std::vector<std::shared_ptr<Component>> Components() const {
            return m_components;
        }

        // Transform Access
        Transform& GetTransform() {
            return m_transform;
        }

        const Transform& GetTransform() const {
            return m_transform;
        }

        // Hierarchy
        void AddChild(std::unique_ptr<Entity> child) {
            child->m_parent = this;
            m_children.push_back(std::move(child));
        }

        const std::vector<std::unique_ptr<Entity>>& GetChildren() const {
            return m_children;
        }

        Entity* GetParent() const {
            return m_parent;
        }

        // Components
        void AddComponent(std::shared_ptr<Component> component) {
            component->OnAttach(this);
            m_components.push_back(std::move(component));
        }

        template<typename T>
        T* GetComponent() const {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
            for (const auto& comp : m_components) {
                if (auto casted = dynamic_cast<T*>(comp.get())) {
                    return casted;
                }
            }
            return nullptr;
        }

    void Start() {
        for (auto& component: m_components) {
            component->Start();
        }

        for (auto& child : m_children) {
            child->Start();
        }
    }

    // Update
    void Update() {
        // Update own components
        for (auto& component : m_components) {
            component->Update();
        }

        // Update children
        for (auto& child: m_children) {
            child->Update();
        }
    }
};

#endif