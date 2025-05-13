
#include "Scene.h"

#include <iterator>
#include <map>
#include <thread>

#include <glad/gl.h>

#include "Action.h"
#include "Camera.h"
#include "Color.h"
#include "Input.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "MultiMeshRenderer.h"
#include "Shader.h"
#include "TimeUtils.h"
#include "Transform.h"
#include "Window.h"

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

Scene* Scene::activeScene = nullptr;

Action<void()> Scene::startGameObjectCall = Action<void()>();
Action<void()> Scene::updateGameObjectCall = Action<void()>();

Scene::Scene() {
    activeScene = this;
}

Scene::~Scene() {
    delete window;

    for(int i = gameObjects.size()-1; i >= 0; --i) {
        // std::cout << "Deleting GameObject: " << gameObjects[i]->name << std::endl;
        delete gameObjects[i];
    }
}

void Scene::initialize() {
    handleSetup();

    start();
    internalStart();

    while(!window->shouldClose()) {
        internalUpdate();
    }
}

void Scene::handleSetup() {
    window = new Window();
    window->initialize(1200, 600, "Zaephus Engine");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    setupLights();

    sortTransparents();
}

void Scene::internalUpdate() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    startGameObjectCall.invoke();
    updateGameObjectCall.invoke();

    {
#ifdef ENABLE_PROFILING
        ZoneNamedN(UpdateZone, "update", true);
#endif
        update();
    }

    render();

    Time::tick();

    handleDestroyingGameObjects();

    window->presentFrame();

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

void Scene::setupLights() const {
    for(size_t i = 0; i < lights.size(); i++) {
        lights[i]->initialize();
    }
}

void Scene::sortTransparents() {
    std::multimap<float, MeshRenderer*> modelMap;
    for(size_t i = 0; i < transparents.size(); i++) {
        float dist = Vector3::distance(Camera::activeCam->transform->position, transparents[i]->transform->position);
        modelMap.emplace(dist, transparents[i]);
    }

    std::vector<MeshRenderer*> sortedModels;

    for(std::multimap<float, MeshRenderer*>::reverse_iterator it = modelMap.rbegin(); it != modelMap.rend(); ++it) {
        sortedModels.push_back(it->second);
    }

    transparents = sortedModels;
}

void Scene::render() {
#ifdef ENABLE_PROFILING
    ZoneScopedC(0x0062ff);
#endif

    for(size_t i = 0; i < opaques.size(); i++) {
        opaques[i]->getShader()->setVector3("viewPos", Camera::activeCam->transform->position);
        opaques[i]->getShader()->setLight("light", lights[0]);
        opaques[i]->render();
    }

    sortTransparents();

    for(size_t i = 0; i < transparents.size(); i++) {
        transparents[i]->getShader()->setVector3("viewPos", Camera::activeCam->transform->position);
        transparents[i]->getShader()->setLight("light", lights[0]);
        transparents[i]->render();
    }

    for(size_t i = 0; i < multiOpaques.size(); i++) {
        multiOpaques[i]->getShader()->setVector3("viewPos", Camera::activeCam->transform->position);
        multiOpaques[i]->getShader()->setLight("light", lights[0]);
        multiOpaques[i]->render();
    }
}

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
    if(_renderer->getShader()->isTransparent()) {
        transparents.push_back(_renderer);
    }
    else {
        opaques.push_back(_renderer);
    }
}

void Scene::onMeshRendererDestroyed(MeshRenderer* _renderer) {
    std::vector<MeshRenderer*>* modelList;
    if(_renderer->getShader()->isTransparent()) { modelList = &transparents; }
    else { modelList = &opaques; }

    for(int i = 0; i < modelList->size(); i++) {
        if(_renderer == modelList->at(i)) {
            modelList->erase(modelList->begin() + i);
            modelList->shrink_to_fit();
        }
    }
}

void Scene::onMultiMeshRendererCreated(MultiMeshRenderer* _renderer) {
    multiOpaques.push_back(_renderer);
}

void Scene::onMultiMeshRendererDestroyed(MultiMeshRenderer* _renderer) {
    for(int i = 0; i < multiOpaques.size(); i++) {
        if(_renderer == multiOpaques.at(i)) {
            multiOpaques.erase(multiOpaques.begin() + i);
            multiOpaques.shrink_to_fit();
        }
    }
}