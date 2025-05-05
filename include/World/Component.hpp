#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class Entity; // Forward declaration

// Base class for all components
class Component {
    protected:
        Entity* m_owner = nullptr;

    public:
        virtual ~Component() = default;

        // Called when the component is added to an entity
        virtual void OnAttach(Entity* owner) {
            m_owner = owner;
        }

        // Optional start method
        virtual void Start() {};

        // Optional update method
        virtual void Update() {};

        // Getter for owner
        Entity* GetOwner() const {
            return m_owner;
        }
};

#endif