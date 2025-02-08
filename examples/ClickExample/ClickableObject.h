
#pragma once

#include "Color.h"
#include "GameObject.h"

class ClickableObject : public GameObject {
    public:
        void update() override;
        void clicked(int _button) override;

    private:
        Color oldColor;
        Color clickedColor = Color::magenta();

        float maxTime = 2.0f;
        float timer = 0.0f;

        bool isClicked = false;
};