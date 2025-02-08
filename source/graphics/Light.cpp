
#include "Light.h"

#include "MeshRenderer.h"
#include "Shader.h"

Action<void(Light*)> Light::lightCreatedCall = Action<void(Light*)>();

Light::Light() {
    lightCreatedCall.invoke(this);
}
Light::Light(const Color& _c) {
    color = _c;

    lightCreatedCall.invoke(this);
}
Light::Light(const Color& _c, const float _ambientStrength, const float _specularStrength) {
    color = _c;
    ambientStrength = _ambientStrength;
    specularStrength = _specularStrength;

    lightCreatedCall.invoke(this);
}

void Light::initialize() {
    model = MeshRenderer::loadModel(MeshRenderer::cube, Shader::unlitShader(color));
    addComponent(model);
    transform->scale = Vector3(lightSize);
}