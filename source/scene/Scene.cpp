
#include "Scene.h"

#include <thread>

#include "Action.h"
#include "Input.h"
#include "Light.h"
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

    renderer = new Renderer();
}

void Scene::initialize() {
    handleSetup();

    std::thread renderThread(Renderer::initialize, renderer);

    while(!renderer->isInitialized()) {}

    Input::initialize();

    start();
    internalStart();

    while(!getWindow()->shouldClose()) {
        internalUpdate();
    }

    renderThread.join();
    handleExit();
}

void Scene::handleExit() {
    for(int i = gameObjects.size()-1; i >= 0; --i) {
        // std::cout << "Deleting GameObject: " << gameObjects[i]->name << std::endl;
        delete gameObjects[i];
    }

    Input::dispose();

    GameObject::gameObjectCreatedCall.unbind<Scene, &Scene::onGameObjectCreated>(this);
    GameObject::gameObjectDestroyedCall.unbind<Scene, &Scene::onGameObjectDestroyed>(this);
}

Window* Scene::getWindow() const {
    return renderer->window;
}

void Scene::handleSetup() {
    GameObject::gameObjectCreatedCall.bind<Scene, &Scene::onGameObjectCreated>(this);
    GameObject::gameObjectDestroyedCall.bind<Scene, &Scene::onGameObjectDestroyed>(this);
}

void Scene::internalStart() {
    if(shouldRenderAxis) { setupAxis(); }
}

void Scene::internalUpdate() {
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

    while(!renderer->testAndSetReadyForRender()) {}

    notifyEndOfFrame.invoke();

#ifdef ENABLE_PROFILING
    FrameMark;
#endif
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