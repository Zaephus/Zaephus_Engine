
#pragma once

#include <iostream>

#include <ZEngine.h>
#include <ZMath.h>

#include "Noise.h"

class LightingScene final : public Scene {
    DirectionalLight* dirLight = nullptr;
    std::vector<PointLight*> pointLights = std::vector<PointLight*>(4);

    Camera* cam = nullptr;

    GameObject* floor = nullptr;
    GameObject* donut = nullptr;
    GameObject* crate = nullptr;
    GameObject* redGlassBox = nullptr;
    GameObject* blueGlassBox = nullptr;
    GameObject* greenWindow = nullptr;

    public:
        void start() override {
            Bounds::shouldRender = false;
            shouldRenderAxis = false;

            // dirLight = new DirectionalLight();
            // dirLight->name = "dir_light";
            // dirLight->transform->rotate(-55.0f, 30.0f, 0.0f);

            pointLights[0] = new PointLight();
            pointLights[0]->range = 12.0f;
            pointLights[0]->specularStrength = 2.0f;
            pointLights[0]->name = "point_light_0";
            pointLights[0]->transform->position = Vector3(0.0f, 0.2f, 0.0f);

            {
                // pointLights[1] = new PointLight({ 0.8f, 0.1f, 0.0f, 1.0f });
                // pointLights[1]->range = 5.0f;
                // pointLights[1]->intensity = 2.0f;
                // pointLights[1]->name = "point_light_1";
                // pointLights[1]->transform->position = Vector3(0.0f, -0.4f, 0.4f);
                //
                // pointLights[2] = new PointLight({ 0.1f, 0.0f, 0.8f, 1.0f });
                // pointLights[2]->range = 15.0f;
                // pointLights[2]->intensity = 2.5f;
                // pointLights[2]->name = "point_light_2";
                // pointLights[2]->transform->position = Vector3(0.5f, 0.6f, -0.5f);
                //
                // pointLights[3] = new PointLight({ 0.8f, 0.8f, 0.0f, 1.0f });
                // pointLights[3]->range = 8.0f;
                // pointLights[3]->intensity = 2.5f;
                // pointLights[3]->name = "point_light_3";
                // pointLights[3]->transform->position = Vector3(-1.0f, 0.2f, 0.2f);
            }

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.5f, 3.0f };
            cam->addComponent(new DebugCameraController());

            renderer->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            Mesh* quadMesh = ModelLoader::load(ModelLoader::quad)[0]->mesh;

            // Texture2D* floorTexture = Texture2D::load("wood_floor.png");
            // Texture2D* floorTexture = Noise::perlinTexture(256, 256, 100000.0f, 0.0f, 15.0f, 3, 0.5f);
            Texture2D* floorTexture = Noise::whiteNoiseTexture(256, 256);

            Shader* floorShader = Shader::unlitTextureShader(
                floorTexture,
                1.0f
            );

            floor = new GameObject();
            floor->addComponent(new MeshRenderer(quadMesh, floorShader));
            floor->name = "floor";
            floor->transform->position = { 0.0f, -0.5f, 0.0f };
            floor->transform->scale = { 10.0f, 1.0f, 10.0f };

            {
                // Mesh* torusMesh = ModelLoader::load(ModelLoader::torus)[0]->mesh;
                //
                // donut = new GameObject();
                // donut->addComponent(new MeshRenderer(torusMesh, Shader::diffuseShader(
                //     { 0.0f, 0.6f, 0.0f, 1.0f },
                //     4.0f)
                // ));
                // donut->name = "donut";
                //
                // Mesh* cubeMesh = ModelLoader::load(ModelLoader::cube)[0]->mesh;
                //
                // crate = new GameObject();
                // crate->addComponent(new MeshRenderer(cubeMesh, Shader::diffuseTextureShader(
                //     "container2.png",
                //     "container2_specular.png",
                //     32.0f
                // )));
                // crate->name = "crate";
                // crate->transform->position = { -1.5f, 0.75f, -0.5f };
                // crate->transform->scale = Vector3::one() * 0.5f;
                // crate->transform->rotate(20.0f, 35.0f, 0.0f);
                //
                // redGlassBox = new GameObject();
                // redGlassBox->addComponent(new MeshRenderer(cubeMesh, Shader::diffuseShader(
                //     {1.0f, 0.0f, 0.0f, 0.4f},
                //     32.0f
                // )));
                // redGlassBox->name = "redGlassBox";
                // redGlassBox->transform->position = { 1.0f, 0.5f, 0.1f };
                // redGlassBox->transform->scale = Vector3::one() * 0.5f;
                //
                // blueGlassBox = new GameObject();
                // blueGlassBox->addComponent(new MeshRenderer(cubeMesh, Shader::diffuseShader(
                //     {0.0f, 0.0f, 1.0f, 0.4f},
                //     32.0f
                // )));
                // blueGlassBox->name = "blueGlassBox";
                // blueGlassBox->transform->position = { 0.6f, 0.5f, 1.2f };
                // blueGlassBox->transform->scale = Vector3::one() * 0.5f;
            }
        }

        void update() override {
            // pointLights[0]->transform->rotateAround(
            //     Vector3::zero(),
            //     Vector3::up(),
            //     Time::deltaTime * 25.0f,
            //     true
            // );
            //
            // donut->transform->rotate(
            //     Time::deltaTime * 15.0f,
            //     0.0f,
            //     0.0f
            // );
            //
            // crate->transform->rotate(
            //     Time::deltaTime * 15.0f,
            //     Time::deltaTime * 5.0f,
            //     Time::deltaTime * 15.0f
            // );

            // std::cout << Input::getMouseDelta().toString() << std::endl;
        }
};