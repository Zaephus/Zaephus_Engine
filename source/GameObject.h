
#pragma once

#include <Component.h>
#include <Transform.h>
#include <typeinfo>

#include "Action.h"

class Component;
class Bounds;
struct Vector2;

class GameObject {

    public:
        static Action<void(GameObject*)> gameObjectCreatedCall;
        static Action<void(GameObject*)> gameObjectDestroyedCall;

        Transform* transform = new Transform();

        std::string name;

        GameObject();
        virtual ~GameObject();

        void destroy();

        void addComponent(Component* _component);
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

        virtual void clicked(int _button, const Vector3& _hitPos) {}

    protected:
        virtual void start() {}
        virtual void update() {}

    private:
        std::vector<Component*> components;

        void internalStart();
        void internalUpdate();
};