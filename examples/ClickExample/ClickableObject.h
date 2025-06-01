
#pragma once

#include <ZEngineFwd.h>

#include "Color.h"
#include "GameObject.h"

class ClickableObject : public GameObject {
    public:
        ClickableObject();

        void clicked();

    private:
        Shader* shader = nullptr;

        Color oldColor;
        Color clickedColor = Color::magenta();

        float maxTime = 2.0f;
        float timer = 0.0f;

        bool isClicked = false;

        void start() override;
        void update() override;
};