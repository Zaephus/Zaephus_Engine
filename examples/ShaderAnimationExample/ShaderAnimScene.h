
#pragma once

#include <ZEngine.h>
#include <ZMath.h>

class ShaderAnimScene final : public Scene {
    DirectionalLight* light = nullptr;

    Camera* cam = nullptr;

    GameObject* capsule = nullptr;

    public:
        void start() override {
            Bounds::shouldRender = false;
            shouldRenderAxis = false;

            light = new DirectionalLight();
            light->name = "main_light";
            light->transform->rotation = Quaternion::fromEuler({ -45.0f, 30.0f, 0.0f });

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.5f, 4.0f };

            renderer->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            const std::vector<Model*> models = ModelLoader::load(ModelLoader::capsule);
            Mesh* capsuleMesh = models[0]->mesh;

            Shader* capsuleShader = new Shader("CapsuleVertex.glsl", "ZE_DiffuseFragment.glsl");
            capsuleShader->setColor("material.color", Color::green());
            capsuleShader->setFloat("material.shininess", 32.0f);
            capsuleShader->setFloat("speed", 3.5f);

            capsule = new GameObject();
            capsule->addComponent(new MeshRenderer(capsuleMesh, capsuleShader));
            capsule->name = "capsule";
        }

        void update() override {}
};