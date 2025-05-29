
#pragma once

#include "Component.h"

class DebugCameraController : public Component {
    public:
        float moveSpeed = 5.0f;
        float sensitivity = 0.06f;

        void update() override;

    private:
        float yaw = 0.0f;
        float pitch = 0.0f;

        bool mouseDown = false;

        void move() const;
        void rotate();
};