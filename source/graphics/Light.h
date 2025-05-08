
#pragma once

#include "Color.h"
#include "GameObject.h"

template <typename T>
class Action;
class MeshRenderer;

class Light : public GameObject {
    public:
        static Action<void(Light*)> lightCreatedCall;
        static Action<void(Light*)> lightDestroyedCall;

        Color color = Color::white();

        float ambientStrength = 0.2f;
        float specularStrength = 1.0f;

        Light();
        explicit Light(const Color& _c);
        Light(const Color& _c, float _ambientStrength, float _specularStrength);

        ~Light() override;

        void start() override;

    private:
        float lightSize = 0.1f;
};