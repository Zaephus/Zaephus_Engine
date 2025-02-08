
#pragma once

#include "Action.h"
#include "Bounds.h"
#include "Transform.h"

class Component;
struct Vector2;

class GameObject {

    public:
        static Action<void(GameObject*)> gameObjectCreatedCall;
        static bool renderBounds;

        Transform* transform;

        std::string name;
        Bounds bounds = Bounds();

        GameObject();
        virtual ~GameObject();

        virtual void update() {}

        virtual void clicked(int _button) {}

        void addComponent(Component* _component);

        template<typename T>
        T* getComponent() {
            T* result = nullptr;
            for(size_t i = 0; i < components.size(); i++) {
                result = static_cast<T*>(components[i]);
                if(result) { break; }
                // if(typeid(components[i]) == typeid(T*)) {
                // }
            }

            return result;
        }

    private:
        std::vector<Component*> components;

        void onMousePressed(int _button, Vector2 _pos);
};