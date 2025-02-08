
#pragma once

#include <ZMath.h>

#include "Camera.h"
#include "ClickableObject.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "Shader.h"

class ClickExample final : public Scene {
    Light* light = nullptr;

    Camera* cam = nullptr;

    ClickableObject* box = nullptr;
    ClickableObject* donut = nullptr;

    public:
        void start() override {
            GameObject::renderBounds = false;
            shouldRenderAxis = false;

            light = new Light();
            light->transform->position = { 2.0f, 3.0f, 3.0f };

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 12.0f / 6.0f, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.0f, 2.0f };
            cam->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            box = new ClickableObject();
            box->addComponent(MeshRenderer::loadModel(MeshRenderer::cube, Shader::diffuseShader(
                { 0.3f, 0.3f, 0.3f, 1.0f },
                4.0f)
            ));
            box->name = "box";
            box->transform->position = { -0.8f, 0.0f, 0.0f };
            box->transform->scale = Vector3::one() * 0.5f;

            donut = new ClickableObject();
            donut->addComponent(MeshRenderer::loadModel(MeshRenderer::torus, Shader::diffuseShader(
                { 0.0f, 0.6f, 0.0f, 1.0f },
                4.0f)
            ));
            donut->name = "donut";
            donut->bounds.top = 0.15f;
            donut->bounds.bottom = -0.15f;
            donut->transform->position = { 0.8f, 0.0f, 0.0f };
            donut->transform->rotate(90.0f, 0.0f, 0.0f);
        }

        void update() override {
            // donut->transform->rotate(
            //     Time::deltaTime * 15.0f,
            //     0.0f,
            //     0.0f
            // );
        }

        ~ClickExample() override {
            delete cam;
        }
};