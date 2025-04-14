
#pragma once

#include <Bounds.h>
#include <ZMath.h>

#include <Camera.h>
#include <Light.h>
#include <MeshRenderer.h>
#include <Scene.h>
#include <Shader.h>
#include <TimeUtils.h>
#include <Transform.h>

#include "Random.h"

class CubesScene final : public Scene {
    Light* light = nullptr;

    Camera* cam = nullptr;

    std::vector<GameObject*> cubes;

    int cubeAmount = 1000;
    float size = 10.0f;

    public:
        void start() override {
            Bounds::shouldRender = false;
            shouldRenderAxis = false;

            light = new Light();
            light-> name = "main_light";
            // light->transform->position = {1.0f, 2.0f, 3.0f };

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.0f, 35.0f };
            cam->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            for(int i = 0; i < cubeAmount; i++) {
                GameObject* cube = new GameObject();
                cube->addComponent(MeshRenderer::loadModel(MeshRenderer::cube, Shader::diffuseShader(
                    { 0.0f, 0.6f, 0.0f, 1.0f },
                    4.0f)
                ));
                cube->name = std::format("Cube {0}", i);
                cube->transform->position = {
                    Random::range(-size, size),
                    Random::range(-size, size),
                    Random::range(-size, size)
                };
                cubes.push_back(cube);
            }
        }

        void update() override {
            for(int i = 0; i < cubes.size(); i++) {
                cubes[i]->transform->rotate(cubes[i]->transform->position.normalized() * 25.0f * Time::deltaTime);
            }
        }
};