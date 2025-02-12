
#pragma once

#include "Action.h"

class Component;
class Transform;
class Bounds;
struct Vector2;

class GameObject {

    public:
        static Action<void(GameObject*)> gameObjectCreatedCall;
        static bool renderBounds;

        Transform* transform;
        Bounds* bounds;

        std::string name;

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
            }

            return result;
        }

    private:
        std::vector<Component*> components;

        void onMousePressed(int _button, Vector2 _pos);
};