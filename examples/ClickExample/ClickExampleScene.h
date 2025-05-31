
#pragma once

#include <iostream>

#include <ZEngine.h>
#include <ZMath.h>

#include "ClickableObject.h"

class ClickExample final : public Scene {
    PointLight* light = nullptr;

    Camera* cam = nullptr;

    ClickableObject* box = nullptr;
    ClickableObject* donut = nullptr;

    bool isMouseDown = false;

    public:
        void start() override {
            Bounds::shouldRender = false;
            shouldRenderAxis = false;

            light = new PointLight();
            light->name = "light";
            light->transform->position = { 2.0f, 3.0f, 3.0f };

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 100.0f);
            // cam = Camera::createOrthographicCamera(3.0f, 12.0f / 6.0f, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.0f, 2.0f };
            cam->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            const std::vector<Model> cubeModels = ModelLoader::load(ModelLoader::cube);
            Mesh* cubeMesh = cubeModels[0].mesh;

            box = new ClickableObject();
            box->addComponent(new MeshRenderer(cubeMesh, Shader::diffuseShader(
                { 0.3f, 0.3f, 0.3f, 1.0f },
                4.0f)
            ));
            box->name = "box";
            box->transform->position = { -0.8f, 0.0f, 0.0f };
            box->transform->scale = Vector3::one() * 0.5f;

            const std::vector<Model> torusModels = ModelLoader::load(ModelLoader::torus);
            Mesh* torusMesh = torusModels[0].mesh;

            donut = new ClickableObject();
            donut->addComponent(new MeshRenderer(torusMesh, Shader::diffuseShader(
                { 0.0f, 0.6f, 0.0f, 1.0f },
                4.0f)
            ));
            donut->name = "donut";
            donut->getComponent<Bounds>()->top = 0.15f;
            donut->getComponent<Bounds>()->bottom = -0.15f;
            donut->transform->position = { 0.8f, 0.0f, 0.0f };
            donut->transform->rotate(90.0f, 0.0f, 0.0f);

            std::cout << donut->transform->objectMatrix().toString() << std::endl;
            std::cout << sizeof(float) << std::endl;
            std::cout << sizeof(Matrix4x4) << std::endl;

            // std::cout << donut->bounds->intersectsLine({ 0.0f, 0.0f, 1.0f }, Vector3::forward()) << std::endl;
        }

        void update() override {
            // donut->transform->rotate(
            //     Time::deltaTime * 15.0f,
            //     0.0f,
            //     0.0f
            // );

            if(!isMouseDown && Input::isMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
                isMouseDown = true;
                const RayCast3D ray = cam->screenToRay(Input::getMousePosition(), true);

                if(ray.isColliding()) {
                    const GameObject* hitObject = ray.getCollider()->gameObject;

                    if(hitObject == donut) {
                        donut->clicked();
                    }
                    if(hitObject == box) {
                        box->clicked();
                    }
                }
            }

            if(isMouseDown && Input::isMouseUp(GLFW_MOUSE_BUTTON_LEFT)) {
                isMouseDown = false;
            }
        }
};