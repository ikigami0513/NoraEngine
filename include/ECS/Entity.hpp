#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Physics/Transform.hpp"
#include "ECS/Component.hpp"
#include "Api/PythonComponentWrapper.hpp"

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
            m_components.push_back(component);
        }

        template<typename T>
        T* GetComponent() const {
            for (const auto& comp_ptr : m_components) {
                if (T* specific_comp = dynamic_cast<T*>(comp_ptr.get())) {
                    return specific_comp;
                }

                if (PythonComponentWrapper* wrapper = dynamic_cast<PythonComponentWrapper*>(comp_ptr.get())) {
                    py::object py_obj = wrapper->PyComponent();
                    if (py_obj && !py_obj.is_none()) {
                        try {
                            if (py::isinstance<T>(py_obj)) {
                                return py_obj.cast<T*>();
                            }
                        } catch (const py::cast_error& e) {
                            std::cerr << "Pybind11 cast error in Entity::GetComponent<T>: " << e.what() << std::endl;
                        }
                    }
                }
            }

            return nullptr;
        }

    template<typename T>
    std::vector<T*> GetComponents() const {
        std::vector<T*> found_components;
        for (const auto& comp_ptr : m_components) {
            if (T* specific_comp = dynamic_cast<T*>(comp_ptr.get())) {
                found_components.push_back(specific_comp);
            }

            if (PythonComponentWrapper* wrapper = dynamic_cast<PythonComponentWrapper*>(comp_ptr.get())) {
                py::object py_obj = wrapper->PyComponent();
                if (py_obj && !py_obj.is_none()) {
                    try {
                        if (py::isinstance<T>(py_obj)) {
                            found_components.push_back(py_obj.cast<T*>());
                        }
                    } catch (const py::cast_error& e) {
                        std::cerr << "Pybind11 cast error in Entity::GetComponent<T>: " << e.what() << std::endl;
                    }
                }
            }
        }

        return found_components;
    }

    void Start() {
        for (auto& component: m_components) {
            try {
                component->Start();
            }
            catch (const py::cast_error& e) {
                std::cerr << "Error while calling component Start" << std::endl;
            }
        }

        for (auto& child : m_children) {
            child->Start();
        }
    }

    // Update
    void Update() {
        // Update own components
        if (m_parent) {
            m_transform.ComputeModelMatrix(m_parent->GetTransform().GetModelMatrix());
        }
        else {
            m_transform.ComputeModelMatrix();
        }

        for (auto& component : m_components) {
            for (auto& component: m_components) {
                try {
                    component->Update();
                }
                catch (const py::cast_error& e) {
                    std::cerr << "Error while calling component Update" << std::endl;
                }
            }
        }

        // Update children
        for (auto& child: m_children) {
            child->Update();
        }
    }

    void OnCollisionEnter(Entity* other) {
        for (auto& component : m_components) {
            try {
                component->OnCollisionEnter(other);
            }
            catch(const py::cast_error& e) {
                std::cerr << "Error while calling component On Collision Enter" << std::endl;
            }
        }
    }
};

#endif