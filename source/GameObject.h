
#pragma once

#include <Component.h>
#include <map>
#include <typeinfo>

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

        virtual void update();

        virtual void clicked(int _button) {}

        void addComponent(Component* _component);

        template<typename T>
        T* getComponent() {
            T* result = nullptr;
            for(size_t i = 0; i < components.size(); i++) {
                // result = dynamic_cast<T*>(components[i]);
                // if(result) { break; }
                // auto index = components.find(typeid(T));
                // if(index != components.end()) {
                //     return static_cast<T*>(index->second);
                // }
                // std::cout << typeid(static_cast<T*>(components[i])).name() << std::endl;
                // std::cout << typeid(T).name() << std::endl;
                // if(typeid(components[i]) == typeid(T)) {
                //     result = static_cast<T*>(components[i]);
                // }
                if(components[i]->getType() == typeid(T)) {
                    result = static_cast<T*>(components[i]);
                }
            }

            return result;
        }

    private:
        std::vector<Component*> components;

        void onMousePressed(int _button, Vector2 _pos);
};