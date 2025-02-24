
#pragma once

#include "Action.h"
#include "Color.h"
#include "GameObject.h"

class MeshRenderer;

class Light : public GameObject {
    public:
        static Action<void(Light*)> lightCreatedCall;

        Color color = Color::white();

        float ambientStrength = 0.2f;
        float specularStrength = 1.0f;

        Light();
        explicit Light(const Color& _c);
        Light(const Color& _c, float _ambientStrength, float _specularStrength);

        void initialize();

    private:
        float lightSize = 0.1f;

        MeshRenderer* model = nullptr;
};