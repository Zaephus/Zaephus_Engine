
#pragma once

#include "GameObject.h"
#include "Transform.h"

class Component {
    public:
        GameObject* gameObject;
        Transform* transform;
};