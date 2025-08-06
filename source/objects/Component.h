
#pragma once

#include "Object.h"

class GameObject;
class Transform;

class Component : public Object {
    public:
        GameObject* gameObject = nullptr;
        Transform* transform = nullptr;
};