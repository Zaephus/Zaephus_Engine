
#include "DirectionalLight.h"

#include "Action.h"
#include "Color.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "ModelLoader.h"
#include "Shader.h"
#include "Transform.h"

Action<void(DirectionalLight*)> DirectionalLight::dirLightCreatedCall = Action<void(DirectionalLight*)>();
Action<void(DirectionalLight*)> DirectionalLight::dirLightDestroyedCall = Action<void(DirectionalLight*)>();

bool DirectionalLight::renderDebugArrow = false;

DirectionalLight::DirectionalLight()
    : DirectionalLight(Color::white(), 0.2f, 1.0f) {}
DirectionalLight::DirectionalLight(const Color& _c)
    : DirectionalLight(_c, 0.2f, 1.0f) {}
DirectionalLight::DirectionalLight(const Color& _c, const float _ambientStrength, const float _specularStrength) {
    color = _c;
    ambientStrength = _ambientStrength;
    specularStrength = _specularStrength;

    dirLightCreatedCall.invoke(this);
}

DirectionalLight::~DirectionalLight() {
    dirLightDestroyedCall.invoke(this);
}

void DirectionalLight::start() {
    if(renderDebugArrow) {
        Mesh* mesh = ModelLoader::load(ModelLoader::arrow)[0].mesh;
        addComponent(new MeshRenderer(mesh, Shader::unlitShader(color)));
        transform->scale *= debugSize;
    }
}