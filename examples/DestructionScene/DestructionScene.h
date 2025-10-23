
#pragma once

#include <vector>

#include <ZEngine.h>
#include <ZMath.h>

class DestructionScene final : public Scene {
    DirectionalLight* light = nullptr;

    Camera* cam = nullptr;

    Shader* cubeShader = nullptr;
    Mesh* cubeMesh = nullptr;

    std::vector<GameObject*> cubes;
    int cubeAmount = 1000;
    float size = 25.0f;

    public:
        void start() override {
            Bounds::shouldRender = false;
            shouldRenderAxis = false;

            light = new DirectionalLight();
            light->name = "main_light";
            light->transform->rotate(-55.0f, 30.0f, 0.0f);

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 1000.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.0f, 75.0f };
            cam->addComponent(new DebugCameraController());

            renderer->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            cubeShader = Shader::diffuseShader(
                { 0.0f, 0.6f, 0.0f, 1.0f },
                4.0f
            );

            const std::vector<Model*> models = ModelLoader::load(ModelLoader::cube, true);
            cubeMesh = models[0]->mesh;

            // for(int i = 0; i < cubeAmount; i++) {
            //     GameObject* cube = new GameObject;
            //     cube->addComponent(new MeshRenderer(cubeMesh, cubeShader));
            //     cube->transform->position.x = Random::range(-size, size);
            //     cube->transform->position.y = Random::range(-size, size);
            //     cube->transform->position.z = Random::range(-size, size);
            //
            //     cubes.push_back(cube);
            // }
        }

        void update() override {
            // const int num = Random::range(0, cubeAmount-1);
            // cubes[num]->destroy();
            // cubes.erase(cubes.begin() + num);
            //
            // GameObject* cube = new GameObject;
            // cube->addComponent(new MeshRenderer(cubeMesh, cubeShader));
            // cube->transform->position.x = Random::range(-size, size);
            // cube->transform->position.y = Random::range(-size, size);
            // cube->transform->position.z = Random::range(-size, size);
            //
            // cubes.push_back(cube);

            for(int i = 0; i < cubes.size(); i++) {
                cubes[i]->destroy();
            }
            cubes.clear();

            for(int i = 0; i < cubeAmount; i++) {
                GameObject* cube = new GameObject;
                cube->addComponent(new MeshRenderer(cubeMesh, cubeShader));
                cube->transform->position.x = Random::range(-size, size);
                cube->transform->position.y = Random::range(-size, size);
                cube->transform->position.z = Random::range(-size, size);

                cubes.push_back(cube);
            }
        }
};