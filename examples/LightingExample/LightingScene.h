
#pragma once

#include <iostream>

#include <ZMath.h>

#include "Bounds.h"
#include "Camera.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture2D.h"
#include "TimeUtils.h"
#include "Transform.h"

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
            GameObject::renderBounds = false;
            shouldRenderAxis = false;

            light = new Light();
            light->name = "light";
            light->transform->position = Vector3(1.0f, 0.5f, 0.0f);

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.5f, 3.0f };

            cam->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            floor = new GameObject();
            floor->addComponent(MeshRenderer::loadModel(MeshRenderer::quad, Shader::diffuseShader(
                { 0.3f, 0.3f, 0.3f, 1.0f },
                4.0f)
            ));
            floor->name = "floor";
            floor->bounds->top = 0.01f;
            floor->bounds->bottom = -0.01f;
            floor->transform->position = { 0.0f, -0.5f, 0.0f };
            floor->transform->scale = { 10.0f, 1.0f, 10.0f };

            donut = new GameObject();
            donut->addComponent(MeshRenderer::loadModel(MeshRenderer::torus, Shader::diffuseShader(
                { 0.0f, 0.6f, 0.0f, 1.0f },
                4.0f)
            ));
            donut->name = "donut";
            donut->bounds->top = 0.15f;
            donut->bounds->bottom = -0.15f;

            crate = new GameObject();
            crate->addComponent(MeshRenderer::loadModel(MeshRenderer::cube, Shader::textureShader(
                "container2.png",
                "container2_specular.png",
                32.0f
            )));
            crate->name = "crate";
            crate->transform->position = { -1.5f, 0.75f, -0.5f };
            crate->transform->scale = Vector3::one() * 0.5f;
            crate->transform->rotate(20.0f, 35.0f, 0.0f);

            redGlassBox = new GameObject();
            redGlassBox->addComponent(MeshRenderer::loadModel(MeshRenderer::cube, Shader::diffuseShader(
                {1.0f, 0.0f, 0.0f, 0.4f},
                1.0f
            )));
            redGlassBox->name = "redGlassBox";
            redGlassBox->transform->position = { 1.0f, 0.5f, 0.1f };
            redGlassBox->transform->scale = Vector3::one() * 0.5f;

            blueGlassBox = new GameObject();
            blueGlassBox->addComponent(MeshRenderer::loadModel(MeshRenderer::cube, Shader::diffuseShader(
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