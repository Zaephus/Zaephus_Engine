
#pragma once

#include <vector>

#include <ZMath.h>
#include <ZEngine.h>

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

class CubesScene final : public Scene {
    Light* light = nullptr;

    Camera* cam = nullptr;

    Shader* cubeShader = nullptr;

    GameObject* multiCube = nullptr;

    int cubeAmount = 1'000'000;
    float size = 25.0f;

    public:
        void start() override {

            Bounds::shouldRender = false;
            shouldRenderAxis = false;

            light = new Light();
            light-> name = "main_light";

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 1000.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.0f, 75.0f };
            cam->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            cubeShader = Shader::instancedDiffuseShader(
                { 0.0f, 0.6f, 0.0f, 1.0f },
                4.0f
            );

            const std::vector<Model> models = ModelLoader::load(ModelLoader::cube, true);
            Mesh* cubeMesh = models[0].mesh;

            multiCube = new GameObject();
            MultiMeshRenderer* ren = new MultiMeshRenderer(cubeMesh, cubeShader, cubeAmount);
            multiCube->addComponent(ren);
            multiCube->name = "Multicube";

            for(int i = 0; i < cubeAmount; i++) {
                ren->setInstancePosition(i, {
                     Random::range(-size, size),
                     Random::range(-size, size),
                     Random::range(-size, size)
                });
            }
        }

        void update() override {
#ifdef ENABLE_PROFILING
            ZoneScopedN("scene update");
#endif

            MultiMeshRenderer* renderer = multiCube->getComponent<MultiMeshRenderer>();
            renderer->getShader()->setLight("light", light);
            renderer->render();

            // for(size_t i = 0; i < cubeAmount; i++) {
            //     renderer->rotateInstance(i, renderer->getInstancePosition(i).normalized() * 25.0f * Time::deltaTime);
            // }
            // for(int i = 0; i < cubes.size(); i++) {
            //     cubes[i]->transform->rotate(cubes[i]->transform->position.normalized() * 25.0f * Time::deltaTime);
            // }
        }
};