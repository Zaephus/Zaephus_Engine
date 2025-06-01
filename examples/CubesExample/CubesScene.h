
#pragma once

#include <vector>
#include <thread>

#include <ZEngine.h>
#include <ZMath.h>

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

class CubesScene final : public Scene {
    DirectionalLight* light = nullptr;

    Camera* cam = nullptr;

    Shader* cubeShader = nullptr;

    GameObject* multiCube = nullptr;
    MultiMeshRenderer* multiRenderer = nullptr;

    unsigned int cubeAmount = 1'000'000;
    float size = 25.0f;

    unsigned int numThreads = 1;

    public:
        void start() override {

            numThreads = std::thread::hardware_concurrency() - 2;

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

            cubeShader = Shader::instancedDiffuseShader(
                { 0.0f, 0.6f, 0.0f, 1.0f },
                4.0f
            );

            const std::vector<Model> models = ModelLoader::load(ModelLoader::cube, true);
            Mesh* cubeMesh = models[0].mesh;

            multiCube = new GameObject();
            multiRenderer = new MultiMeshRenderer(cubeMesh, cubeShader, cubeAmount);
            multiCube->addComponent(multiRenderer);
            multiCube->name = "Multi-cube";

            for(int i = 0; i < cubeAmount; i++) {
                multiRenderer->setInstancePosition(i, {
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

            std::vector<std::thread> threads;

            const unsigned int chunkSize = cubeAmount / numThreads;

            for(size_t i = 0; i < numThreads; i++) {
                unsigned int start = i * chunkSize;
                unsigned int length = chunkSize;

                threads.emplace_back(&CubesScene::rotateCubes, this, start, length);
            }

            for(std::thread& thread : threads) {
                thread.join();
            }
        }

        void rotateCubes(const int _start, const int _length) const {
            for(size_t i = _start; i < _start + _length; i++) {
                multiRenderer->rotateInstance(i, multiRenderer->getInstancePosition(i).normalized() * 25.0f * Time::deltaTime);
            }
        }
};