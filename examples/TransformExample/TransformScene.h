
#pragma once

#include <ZEngine.h>
#include <ZMath.h>

class TransformScene final : public Scene {
    Light* light = nullptr;

    Camera* cam = nullptr;

    GameObject* parent = nullptr;
    GameObject* child = nullptr;

    public:
        void start() override {
            Bounds::shouldRender = false;
            shouldRenderAxis = false;

            light = new Light();
            light-> name = "main_light";
            light->transform->position = {1.0f, 2.0f, 3.0f };

            cam = Camera::createPerspectiveCamera(45.0f * ZMath::deg2rad, 0.1f, 100.0f);
            cam->name = "camera";
            cam->transform->position = { 0.0f, 0.0f, 10.5f };

            renderer->setClearColor(0.2f, 0.25f, 0.4f, 1.0f);

            const std::vector<Model> models = ModelLoader::load(ModelLoader::cube, true);
            Mesh* cubeMesh = models[0].mesh;

            parent = new GameObject();
            parent->addComponent(new MeshRenderer(cubeMesh, Shader::diffuseShader(
                { 0.0f, 0.6f, 0.0f, 1.0f },
                4.0f)
            ));
            parent->name = "parent";
            parent->transform->rotate(0.0f, 30.0f, 0.0f);

            child = new GameObject();
            child->addComponent(new MeshRenderer(cubeMesh, Shader::diffuseShader(
                Color::blue(),
                32.0f)
            ));
            child->name = "child";
            child->transform->parent = parent->transform;
            child->transform->position = { -0.75f, 0.0f, 0.0f };
            child->transform->scale = Vector3::one() * 0.5f;

            // child->getComponent<MeshRenderer>()->getShader()->setColor("material.color", Color::blue());

        }

        void update() override {
            parent->transform->rotate(0.0f, 0.0f, 6.0f * Time::deltaTime);
            child->transform->rotate(32.0f * Time::deltaTime, 0.0f, 0.0f);
        }
};