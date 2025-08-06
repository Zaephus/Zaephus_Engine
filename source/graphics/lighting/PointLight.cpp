
#include "PointLight.h"

#include "Action.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Model.h"
#include "ModelLoader.h"
#include "Shader.h"
#include "Transform.h"

Action<void(PointLight*)> PointLight::pointLightCreatedCall = Action<void(PointLight*)>();
Action<void(PointLight*)> PointLight::pointLightDestroyedCall = Action<void(PointLight*)>();

PointLight::PointLight()
    : PointLight(Color::white(), 0.2f, 1.0f) {}
PointLight::PointLight(const Color& _c)
    : PointLight(_c, 0.2f, 1.0f) {}
PointLight::PointLight(const Color& _c, const float _ambientStrength, const float _specularStrength) {
    color = _c;
    ambientStrength = _ambientStrength;
    specularStrength = _specularStrength;

    pointLightCreatedCall.invoke(this);
}

PointLight::~PointLight() {
    pointLightDestroyedCall.invoke(this);
}

void PointLight::start() {
    Mesh* mesh = ModelLoader::load(ModelLoader::cube)[0]->mesh;
    addComponent(new MeshRenderer(mesh, Shader::unlitShader(color)));

    transform->scale = Vector3(lightSize);
}

void PointLight::calculateAttenuation() {
    int min = 0;
    int max = 1;
    for(int i = 0; i < 12; i++) {
        if(attValues[i].dist > range) {
            if(i > 0) {
                min = i-1;
                max = i;
            }
            break;
        }
    }

    if(attValues[min].dist == range) {
        constantAtt = attValues[min].constant;
        linearAtt = attValues[min].linear;
        quadraticAtt = attValues[min].quadratic;
    }

    const float t = (range - attValues[min].dist) / (attValues[max].dist - attValues[min].dist);
    constantAtt =  ZMath::lerpUnclamped(attValues[min].constant,  attValues[max].constant, t);
    linearAtt =    ZMath::lerpUnclamped(attValues[min].linear,    attValues[max].linear, t);
    quadraticAtt = ZMath::lerpUnclamped(attValues[min].quadratic, attValues[max].quadratic, t);
}