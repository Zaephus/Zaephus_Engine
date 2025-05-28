
#pragma once

#include <iostream>

#include <ZEngine.h>
#include <ZMath.h>

class LightingScene final : public Scene {

    Light* light = nullptr;

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

            light = new Light();
            light->name = "light";
            light->transform->position = Vector3(1.0f, 0.5f, 0.0f);

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.5f, 3.0f };

            renderer->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            Mesh* quadMesh = ModelLoader::load(ModelLoader::quad)[0].mesh;

            floor = new GameObject();
            floor->addComponent(new MeshRenderer(quadMesh, Shader::diffuseShader(
                { 0.3f, 0.3f, 0.3f, 1.0f },
                4.0f)
            ));
            floor->name = "floor";
            floor->transform->position = { 0.0f, -0.5f, 0.0f };
            floor->transform->scale = { 10.0f, 1.0f, 10.0f };

            Mesh* torusMesh = ModelLoader::load(ModelLoader::torus)[0].mesh;

            donut = new GameObject();
            donut->addComponent(new MeshRenderer(torusMesh, Shader::diffuseShader(
                { 0.0f, 0.6f, 0.0f, 1.0f },
                4.0f)
            ));
            donut->name = "donut";

            Mesh* cubeMesh = ModelLoader::load(ModelLoader::cube)[0].mesh;

            crate = new GameObject();
            crate->addComponent(new MeshRenderer(cubeMesh, Shader::textureShader(
                "container2.png",
                "container2_specular.png",
                32.0f
            )));
            crate->name = "crate";
            crate->transform->position = { -1.5f, 0.75f, -0.5f };
            crate->transform->scale = Vector3::one() * 0.5f;
            crate->transform->rotate(20.0f, 35.0f, 0.0f);

            redGlassBox = new GameObject();
            redGlassBox->addComponent(new MeshRenderer(cubeMesh, Shader::diffuseShader(
                {1.0f, 0.0f, 0.0f, 0.4f},
                1.0f
            )));
            redGlassBox->name = "redGlassBox";
            redGlassBox->transform->position = { 1.0f, 0.5f, 0.1f };
            redGlassBox->transform->scale = Vector3::one() * 0.5f;

            blueGlassBox = new GameObject();
            blueGlassBox->addComponent(new MeshRenderer(cubeMesh, Shader::diffuseShader(
                {0.0f, 0.0f, 1.0f, 0.4f},
                1.0f
            )));
            blueGlassBox->name = "blueGlassBox";
            blueGlassBox->transform->position = { 0.6f, 0.5f, 1.2f };
            blueGlassBox->transform->scale = Vector3::one() * 0.5f;
        }

        void update() override {
            light->transform->rotateAround(
                Vector3::zero(),
                Vector3::up(),
                Time::deltaTime * 25.0f,
                true
            );

            donut->transform->rotate(
                Time::deltaTime * 15.0f,
                0.0f,
                0.0f
            );

            crate->transform->rotate(
                Time::deltaTime * 15.0f,
                Time::deltaTime * 5.0f,
                Time::deltaTime * 15.0f
            );
        }
};