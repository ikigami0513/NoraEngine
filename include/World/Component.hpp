#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>

class Entity; // Forward declaration

// Base class for all components
class Component {
    protected:
        Entity* m_owner = nullptr;

    public:
        virtual ~Component() = default;

        // Optional start method
        virtual void Start() {};

        // Optional update method
        virtual void Update() {};

        // Getter for owner
        Entity* GetOwner() const {
            return m_owner;
        }

        void SetOwner(Entity* owner) {
            m_owner = owner;
        }
};

#endif