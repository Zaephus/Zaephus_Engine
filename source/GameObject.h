
#pragma once

#include <Component.h>
#include <typeinfo>

#include "Action.h"

class Component;
class Transform;
class Bounds;
struct Vector2;

class GameObject {

    public:
        static Action<void(GameObject*)> gameObjectCreatedCall;
        static Action<void(GameObject*)> gameObjectDestroyedCall;
        static bool renderBounds;

        Transform* transform;
        Bounds* bounds;

        std::string name;

        GameObject();
        virtual ~GameObject();

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

    protected:
        virtual void start() {}
        virtual void update() {}

        virtual void clicked(int _button) {}

    private:
        std::vector<Component*> components;

        void internalStart();
        void internalUpdate();

        void onMousePressed(int _button, Vector2 _pos);
};