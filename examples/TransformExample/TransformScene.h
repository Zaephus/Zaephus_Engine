
#pragma once

#include <ZMath.h>

#include <Camera.h>
#include <Light.h>
#include <MeshRenderer.h>
#include <Scene.h>
#include <Shader.h>
#include <Transform.h>

class TransformScene final : public Scene {
    Light* light = nullptr;

    Camera* cam = nullptr;

    GameObject* parent = nullptr;

    public:
        void start() override {
            GameObject::renderBounds = false;
            shouldRenderAxis = false;

            light = new Light();
            light-> name = "main_light";
            light->transform->position = {1.0f, 2.0f, 3.0f };

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 12.0f / 6.0f, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.0f, 2.0f };
            cam->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            parent = new GameObject();
            parent->addComponent(MeshRenderer::loadModel(MeshRenderer::cube, Shader::diffuseShader(
                { 0.0f, 0.6f, 0.0f, 1.0f },
                4.0f)
            ));
            parent->name = "parent";
        }

        void update() override {

        }
};