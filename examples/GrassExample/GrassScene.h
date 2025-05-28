
#pragma once

#include <ZMath.h>
#include <ZEngine.h>

class GrassScene final : public Scene {
    Light* light = nullptr;

    Camera* cam = nullptr;

    GameObject* ground = nullptr;

    GameObject* grass = nullptr;
    MultiMeshRenderer* grassRenderer = nullptr;

    int grassAmount = 100'000;

    float groundSize = 100.0f;

    public:
        void start() override {
            Bounds::shouldRender = false;
            shouldRenderAxis = false;

            light = new Light();
            light->name = "main_light";
            light->transform->position = { 1.0f, 4.0f, -10.0f };

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
            ground->transform->scale = { groundSize, 1.0f, groundSize };

            Mesh* grassMesh = ModelLoader::load("grass_blade.obj")[0].mesh;

            Shader* grassShader = new Shader("GrassVertex.glsl", "GrassFragment.glsl");
            grassShader->setColor("material.color", Color::green());
            grassShader->setFloat("material.shininess", 1.0f);
            grassShader->setFloat("speed", 0.8f);
            grassShader->setFloat("range", 0.08f);

            grass = new GameObject();

            grassRenderer = new MultiMeshRenderer(grassMesh, grassShader, grassAmount);
            grass->addComponent(grassRenderer);
            grass->name = "grass";
            // grass->transform->rotate(0.0f, -90.0f, 0.0f);

            for(int i = 0; i < grassAmount; i++) {
                grassRenderer->setInstancePosition(i, {
                    Random::range(-groundSize*0.5f, groundSize*0.5f),
                    0.0f,
                    Random::range(-groundSize*0.5f, groundSize*0.5f)
                });
                grassRenderer->scaleInstance(i, {
                    Random::range(0.8f, 3.2f),
                    Random::range(0.8f, 3.2f),
                    Random::range(0.8f, 3.2f)
                });
                grassRenderer->setInstanceRotation(i, { 0.0f, -90.0f, 0.0f });
            }

            std::cout << grassRenderer->getInstanceScale(1).toString() << std::endl;
        }

        void update() override {}
};