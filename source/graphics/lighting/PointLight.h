
#pragma once

#include "Color.h"
#include "GameObject.h"

template <typename T>
class Action;
class MeshRenderer;

class PointLight : public GameObject {
    public:
        static Action<void(PointLight*)> pointLightCreatedCall;
        static Action<void(PointLight*)> pointLightDestroyedCall;

        Color color = Color::white();

        float range = 7.0f;
        float intensity = 1.0f;

        float ambientStrength = 0.2f;
        float specularStrength = 1.0f;

        float constantAtt = 0.0f;
        float linearAtt = 0.0f;
        float quadraticAtt = 0.0f;

        PointLight();
        explicit PointLight(const Color& _c);
        PointLight(const Color& _c, float _ambientStrength, float _specularStrength);

        void start() override;

        void calculateAttenuation();

    private:
        struct Attenuation {
            float dist;
            float constant;
            float linear;
            float quadratic;
        };

        // Attenuation values supplied by OGRE 3D wiki:
        // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
        Attenuation attValues[12] = {
            { 7.0f,    1.0f, 0.7f,    1.8f },
            { 13.0f,   1.0f, 0.35f,   0.44f },
            { 20.0f,   1.0f, 0.22f,   0.20f },
            { 32.0f,   1.0f, 0.14f,   0.07f },
            { 50.0f,   1.0f, 0.09f,   0.032f },
            { 65.0f,   1.0f, 0.07f,   0.017f },
            { 100.0f,  1.0f, 0.045f,  0.0075f },
            { 160.0f,  1.0f, 0.027f,  0.0028f },
            { 200.0f,  1.0f, 0.022f,  0.0019f },
            { 325.0f,  1.0f, 0.014f,  0.0007f },
            { 600.0f,  1.0f, 0.007f,  0.0002f },
            { 3250.0f, 1.0f, 0.0014f, 0.000007f }
        };

        float lightSize = 0.1f;

        ~PointLight() override;
};