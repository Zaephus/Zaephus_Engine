
#pragma once

#include "Color.h"
#include "GameObject.h"

template <typename T>
class Action;
class MeshRenderer;

class DirectionalLight : public GameObject {
    public:
        static Action<void(DirectionalLight*)> dirLightCreatedCall;
        static Action<void(DirectionalLight*)> dirLightDestroyedCall;

        static bool renderDebugArrow;

        Color color = Color::white();

        float ambientStrength = 0.2f;
        float specularStrength = 1.0f;

        DirectionalLight();
        explicit DirectionalLight(const Color& _c);
        DirectionalLight(const Color& _c, float _ambientStrength, float _specularStrength);

        ~DirectionalLight() override;

        void start() override;

    private:
        float debugSize = 0.4f;
};