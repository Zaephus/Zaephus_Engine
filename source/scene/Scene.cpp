
#include "Scene.h"

#include <thread>

#include "Action.h"
#include "Input.h"
#include "GameObject.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "ModelLoader.h"
#include "Renderer.h"
#include "Shader.h"
#include "TimeUtils.h"
#include "Transform.h"

#include <tracy/Tracy.hpp>

Scene* Scene::activeScene = nullptr;

Action<void()> Scene::startObjectCall = Action<void()>();
Action<void()> Scene::updateObjectCall = Action<void()>();
Action<void()> Scene::destroyObjectCall = Action<void()>();

Scene::Scene() {
    activeScene = this;

    renderer = new Renderer();
}

void Scene::initialize() {
    handleSetup();

    std::thread renderThread(&Renderer::initialize, renderer);

    while(!renderer->isInitialized()) {}

    Input::initialize();

    start();
    internalStart();

    while(!renderer->shouldExit) {
        internalUpdate();
    }

    handleExit();

    renderThread.join();
    delete renderer;
}

void Scene::handleSetup() {
    GameObject::gameObjectCreatedCall.bind<Scene, &Scene::onGameObjectCreated>(this);
    GameObject::gameObjectDestroyedCall.bind<Scene, &Scene::onGameObjectDestroyed>(this);
}

void Scene::handleExit() {
    for(size_t i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->destroy();
    }
    destroyObjectCall.invoke();

    Input::dispose();
    ModelLoader::dispose();

    renderer->setReadyForCleanup();

    GameObject::gameObjectCreatedCall.unbind<Scene, &Scene::onGameObjectCreated>(this);
    GameObject::gameObjectDestroyedCall.unbind<Scene, &Scene::onGameObjectDestroyed>(this);
}

Window* Scene::getWindow() const {
    return renderer->window;
}

void Scene::internalStart() {
    if(shouldRenderAxis) { setupAxis(); }
}

void Scene::internalUpdate() {
    startObjectCall.invoke();
    updateObjectCall.invoke();

    {
        ZoneNamedN(UpdateZone, "Scene::Update", true);
        update();
    }

    Time::tick();

    while(!renderer->isReadyForRender()) {}

    destroyObjectCall.invoke();

    while(!renderer->testAndSetReadyForRender()) {}

    notifyEndOfFrame.invoke();

    FrameMark;
}

void Scene::setupAxis() {
    Shader* xShader = Shader::unlitShader(Color::red());
    Shader* yShader = Shader::unlitShader(Color::green());
    Shader* zShader = Shader::unlitShader(Color::blue());

    Mesh* cubeMesh = ModelLoader::load(ModelLoader::cube)[0]->mesh;

    constexpr float lineDiameter = 0.005f;
    constexpr float cubeSize = 0.05f;

    GameObject* xLine = new GameObject();
    xLine->addComponent(new MeshRenderer(cubeMesh, xShader));
    xLine->transform->scale = { 1000.0f, lineDiameter, lineDiameter };

    GameObject* xCube = new GameObject();
    xCube->addComponent(new MeshRenderer(cubeMesh, xShader));
    xCube->transform->position = { 1.0f, 0.0f, 0.0f };
    xCube->transform->scale *= cubeSize;

    GameObject* yLine = new GameObject();
    yLine->addComponent(new MeshRenderer(cubeMesh, yShader));
    yLine->transform->scale = { lineDiameter, 1000.0f, lineDiameter };

    GameObject* yCube = new GameObject();
    yCube->addComponent(new MeshRenderer(cubeMesh, yShader));
    yCube->transform->position = { 0.0f, 1.0f, 0.0f };
    yCube->transform->scale *= cubeSize;

    GameObject* zLine = new GameObject();
    zLine->addComponent(new MeshRenderer(cubeMesh, zShader));
    zLine->transform->scale = { lineDiameter, lineDiameter, 1000.0f };

    GameObject* zCube = new GameObject();
    zCube->addComponent(new MeshRenderer(cubeMesh, zShader));
    zCube->transform->position = { 0.0f, 0.0f, 1.0f };
    zCube->transform->scale *= cubeSize;
}

void Scene::onGameObjectCreated(GameObject* _gameObject) {
    gameObjects.push_back(_gameObject);
}

void Scene::onGameObjectDestroyed(GameObject* _gameObject) {
    ZoneScopedN("Scene::OnGameObjectDestroyed");

    for(size_t i = 0; i < gameObjects.size(); i++) {
        if(_gameObject == gameObjects[i]) {
            gameObjects.erase(gameObjects.begin() + i);
            gameObjects.shrink_to_fit();
            return;
        }
    }
}