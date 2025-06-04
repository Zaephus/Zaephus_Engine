
#pragma once

#include <Component.h>
#include <typeinfo>
#include <vector>

#include "Destructible.h"
#include "Object.h"

template <typename T>
class Action;

class Transform;
class Component;
class Bounds;
struct Vector2;

class GameObject : public Object, public Destructible {

    public:
        static Action<void(GameObject*)> gameObjectCreatedCall;
        static Action<void(GameObject*)> gameObjectDestroyedCall;

        Transform* transform = nullptr;

        GameObject();

        /**
         * Marks this game object to be destroyed.
         * @note The game object is not destroyed immediately in order to avoid access violations.
         * It is destroyed before the start of the next frame.
         */
        // void destroy();

        void markForDestruction() override;

        /**
         * Adds a component to the game object.
         * @param _component The component you want to add.
         */
        void addComponent(Component* _component);

        /**
         * Removes and destroys a component from the game object.
         * @param _component The component you want to remove.
         */
        void removeComponent(Component* _component);

        /**
         * Finds and returns a component of type T.
         * @tparam T Component type.
         * @return First component of type T, nullptr if no component of that type was found.
         */
        template<typename T>
        T* getComponent() {
            for(size_t i = 0; i < components.size(); i++) {
                if(components[i]->getType() == typeid(T)) {
                    return static_cast<T*>(components[i]);
                }
            }

            return nullptr;
        }

    private:
        std::vector<Component*> components;

        void destroy() override;
};