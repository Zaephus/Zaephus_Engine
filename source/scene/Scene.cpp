
#include "Scene.h"

#include <iterator>
#include <thread>

#include "Action.h"
#include "Input.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "MultiMeshRenderer.h"
#include "RenderBuffer.h"
#include "Renderer.h"
#include "TimeUtils.h"
#include "Window.h"

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

Scene* Scene::activeScene = nullptr;

Action<void()> Scene::startObjectCall = Action<void()>();
Action<void()> Scene::updateObjectCall = Action<void()>();

Scene::Scene() {
    activeScene = this;
    renderBuffer = new RenderBuffer();

    renderer = new Renderer();
}

Scene::~Scene() {
    for(int i = gameObjects.size()-1; i >= 0; --i) {
        // std::cout << "Deleting GameObject: " << gameObjects[i]->name << std::endl;
        delete gameObjects[i];
    }
}

void Scene::initialize() {
    handleSetup();

    renderer->setRenderBuffer(renderBuffer);
    std::thread renderThread(Renderer::initialize, renderer);

    while(!renderer->hasFinishedSetup) {}

    start();
    internalStart();

    while(!getWindow()->shouldClose()) {
        internalUpdate();

#ifdef ENABLE_PROFILING
        FrameMark;
#endif
    }

    renderThread.join();
}

Window* Scene::getWindow() const {
    return renderer->window;
}

void Scene::handleSetup() {
    Input::initialize();

    GameObject::gameObjectCreatedCall.bind<Scene, &Scene::onGameObjectCreated>(this);
    GameObject::gameObjectDestroyedCall.bind<Scene, &Scene::onGameObjectDestroyed>(this);

    Light::lightCreatedCall.bind<Scene, &Scene::onLightCreated>(this);
    Light::lightDestroyedCall.bind<Scene, &Scene::onLightDestroyed>(this);

    MeshRenderer::meshRendererCreatedCall.bind<Scene, &Scene::onMeshRendererCreated>(this);
    MeshRenderer::meshRendererDestroyedCall.bind<Scene, &Scene::onMeshRendererDestroyed>(this);

    MultiMeshRenderer::multiMeshRendererCreatedCall.bind<Scene, &Scene::onMultiMeshRendererCreated>(this);
    MultiMeshRenderer::multiMeshRendererDestroyedCall.bind<Scene, &Scene::onMultiMeshRendererDestroyed>(this);
}

void Scene::internalStart() {
    if(shouldRenderAxis) { setupAxis(); }
}

void Scene::internalUpdate() {
#ifdef ENABLE_PROFILING
    tracy::SetThreadName("Update Thread");
#endif

    startObjectCall.invoke();
    updateObjectCall.invoke();

    {
#ifdef ENABLE_PROFILING
        ZoneNamedN(UpdateZone, "update", true);
#endif
        update();
    }

    Time::tick();

    handleDestroyingGameObjects();

    transferRenderData();
}

void Scene::transferRenderData() const {
    renderBuffer->bind();

    renderBuffer->setMeshRenderers(meshRenderers);
    renderBuffer->setMultiMeshRenderers(multiMeshRenderers);
    renderBuffer->setLights(lights);

    renderBuffer->unbind();
}

void Scene::handleDestroyingGameObjects() {
    for(size_t i = 0; i < gameObjectsToDestroy.size(); i++) {
        delete gameObjectsToDestroy[i];
    }

    gameObjectsToDestroy.clear();
}

void Scene::setupAxis() {}

void Scene::onGameObjectCreated(GameObject* _gameObject) {
    gameObjects.push_back(_gameObject);
}

void Scene::onGameObjectDestroyed(GameObject* _gameObject) {
    gameObjectsToDestroy.push_back(_gameObject);

    for(size_t i = 0; i < gameObjects.size(); i++) {
        if(_gameObject == gameObjects[i]) {
            gameObjects.erase(gameObjects.begin() + i);
            gameObjects.shrink_to_fit();
            return;
        }
    }
}

void Scene::onLightCreated(Light* _light) {
    lights.push_back(_light);
}

void Scene::onLightDestroyed(Light* _light) {
    for(size_t i = 0; i < lights.size(); i++) {
        if(_light == lights[i]) {
            lights.erase(lights.begin() + i);
            lights.shrink_to_fit();
        }
    }
}


void Scene::onMeshRendererCreated(MeshRenderer* _renderer) {
    meshRenderers.push_back(_renderer);
}

void Scene::onMeshRendererDestroyed(MeshRenderer* _renderer) {
    for(int i = 0; i < meshRenderers.size(); i++) {
        if(_renderer == meshRenderers.at(i)) {
            meshRenderers.erase(meshRenderers.begin() + i);
            meshRenderers.shrink_to_fit();
        }
    }
}

void Scene::onMultiMeshRendererCreated(MultiMeshRenderer* _renderer) {
    multiMeshRenderers.push_back(_renderer);
}

void Scene::onMultiMeshRendererDestroyed(MultiMeshRenderer* _renderer) {
    for(int i = 0; i < multiMeshRenderers.size(); i++) {
        if(_renderer == multiMeshRenderers.at(i)) {
            multiMeshRenderers.erase(multiMeshRenderers.begin() + i);
            multiMeshRenderers.shrink_to_fit();
        }
    }
}