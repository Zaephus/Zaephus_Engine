
#pragma once

namespace std { class type_info; }
class GameObject;
class Transform;

class Component {
    public:
        GameObject* gameObject = nullptr;
        Transform* transform = nullptr;

        virtual ~Component() = default;

        virtual void start() {}
        virtual void update() {}

        virtual const std::type_info& getType();
};