
#pragma once

#include <Component.h>
#include <typeinfo>

#include "Action.h"
#include "Object.h"

class Transform;
class Component;
class Bounds;
struct Vector2;

class GameObject : public Object {

    public:
        static Action<void(GameObject*)> gameObjectCreatedCall;
        static Action<void(GameObject*)> gameObjectDestroyedCall;

        Transform* transform = nullptr;

        GameObject();
        ~GameObject() override;

        void destroy();

        void addComponent(Component* _component);
        void addComponents(const std::vector<Component*>& _components);

        void removeComponent(const Component* _component);

        template<typename T>
        T* getComponent() {
            T* result = nullptr;
            for(size_t i = 0; i < components.size(); i++) {
                if(components[i]->getType() == typeid(T)) {
                    result = static_cast<T*>(components[i]);
                }
            }

            return result;
        }

    protected:
        void start() override {}
        void update() override {}

    private:
        std::vector<Component*> components;
};