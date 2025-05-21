
#pragma once

#include <ZMath.h>
#include <ZEngine.h>

class GrassScene final : public Scene {
    Light* light = nullptr;

    Camera* cam = nullptr;

    GameObject* ground = nullptr;
    GameObject* grass = nullptr;

    public:
        void start() override {
            Bounds::shouldRender = false;
            shouldRenderAxis = false;

            light = new Light();
            light->name = "main_light";
            light->transform->position = { 1.0f, 4.0f, -4.0f };

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 2.5f, 0.0f };

            renderer->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            Mesh* groundMesh = ModelLoader::load(ModelLoader::quad)[0].mesh;

            Shader* groundShader = Shader::diffuseShader(
                {46.0f / 255.0f, 35.0f / 255.0f, 19.0f / 255.0f, 1.0f},
                1.0f
            );

            ground = new GameObject();
            ground->addComponent(new MeshRenderer(groundMesh, groundShader));
            ground->transform->scale = { 100.0f, 1.0f, 100.0f };

            Mesh* grassMesh = ModelLoader::load("grass_blade.obj")[0].mesh;

            Shader* grassShader = new Shader("GrassVertex.glsl", "ZE_DiffuseFragment.glsl");
            grassShader->setColor("material.color", Color::green());
            grassShader->setFloat("material.shininess", 32.0f);
            grassShader->setFloat("speed", 0.8f);

            grass = new GameObject();
            grass->addComponent(new MeshRenderer(grassMesh, grassShader));
            grass->transform->position = { 0.0f, 0.0f, -10.0f };
            grass->transform->rotate(0.0f, -90.0f, 0.0f);
        }

        void update() override {}
};