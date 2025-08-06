
#pragma once

#include <ZEngine.h>
#include <ZMath.h>

#include "ClickableObject.h"

class ClickExample final : public Scene {
    DirectionalLight* light = nullptr;

    Camera* cam = nullptr;

    ClickableObject* box = nullptr;
    ClickableObject* donut = nullptr;

    bool isMouseDown = false;

    public:
        void start() override {
            Bounds::shouldRender = true;
            shouldRenderAxis = false;

            light = new DirectionalLight();
            light->name = "light";
            light->transform->rotation = Quaternion::fromEuler({ -45.0f, 30.0f, 0.0f });

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.0f, 2.0f };

            renderer->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            Mesh* cubeMesh = ModelLoader::load(ModelLoader::cube)[0]->mesh;

            box = new ClickableObject();
            box->addComponent(new MeshRenderer(cubeMesh, Shader::diffuseShader(
                { 0.3f, 0.3f, 0.3f, 1.0f },
                4.0f)
            ));
            box->name = "box";
            box->transform->position = { -0.8f, 0.0f, 0.0f };
            box->transform->scale = Vector3::one() * 0.5f;

            Mesh* torusMesh = ModelLoader::load(ModelLoader::torus)[0]->mesh;

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