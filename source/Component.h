
#pragma once

#include <typeinfo>

class GameObject;
class Transform;

class Component {
    public:

        GameObject* gameObject;
        Transform* transform;

        virtual ~Component() = default;

        virtual const std::type_info& getType();
};