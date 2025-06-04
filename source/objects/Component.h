
#pragma once

#include "Destructible.h"
#include "Object.h"

class GameObject;
class Transform;

class Component : public Object, public Destructible {
    public:
        GameObject* gameObject = nullptr;
        Transform* transform = nullptr;

        Component();
        explicit Component(Action<void()>* _destroyCall);

    protected:
        ~Component() override {}
};