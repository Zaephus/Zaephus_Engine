
#include "Scene.h"

#include <iterator>
#include <map>

#include <glad/gl.h>

#include "Action.h"
#include "Camera.h"
#include "Color.h"
#include "Input.h"
#include "Light.h"
#include "MeshRenderer.h"
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

    MeshRenderer::modelCreatedCall.bind<Scene, &Scene::onModelCreated>(this);
    MeshRenderer::modelDestroyedCall.bind<Scene, &Scene::onModelDestroyed>(this);
}

void Scene::internalStart() {
    if(shouldRenderAxis) { setupAxis(); }
    setupLights();

    sortTransparents();
}

void Scene::internalUpdate() {
#ifdef ENABLE_PROFILING
    ZoneScoped;
#endif

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    startGameObjectCall.invoke();
    updateGameObjectCall.invoke();
#ifdef ENABLE_PROFILING
    ZoneNamedN(UpdateZone, "update", true);
#endif
    update();

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

void Scene::setupAxis() {
    // xLine = MeshRenderer::loadModel(MeshRenderer::cube);
    // xLine->setShader(Shader::unlitShader(Color::red()));
    // xLine->transform->scale = { 100.0f, 0.01f, 0.01f };
    //
    // xCube = MeshRenderer::loadModel(MeshRenderer::cube);
    // xCube->setShader(Shader::unlitShader(Color::red()));
    // xCube->transform->position = { 1.0f, 0.0f, 0.0f };
    // xCube->transform->scale = { 0.05f, 0.05f, 0.05f };
    //
    // yLine = MeshRenderer::loadModel(MeshRenderer::cube);
    // yLine->setShader(Shader::unlitShader(Color::green()));
    // yLine->transform->scale = { 0.01f, 100.0f, 0.01f };
    //
    // yCube = MeshRenderer::loadModel(MeshRenderer::cube);
    // yCube->setShader(Shader::unlitShader(Color::green()));
    // yCube->transform->position = { 0.0f, 1.0f, 0.0f };
    // yCube->transform->scale = { 0.05f, 0.05f, 0.05f };
    //
    // zLine = MeshRenderer::loadModel(MeshRenderer::cube);
    // zLine->setShader(Shader::unlitShader(Color::blue()));
    // zLine->transform->scale = { 0.01f, 0.01f, 100.0f };
    //
    // zCube = MeshRenderer::loadModel(MeshRenderer::cube);
    // zCube->setShader(Shader::unlitShader(Color::blue()));
    // zCube->transform->position = { 0.0f, 0.0f, 1.0f };
    // zCube->transform->scale = { 0.05f, 0.05f, 0.05f };
}

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
        if(Camera::activeCam->transform->hasChanged) {
            opaques[i]->getShader()->setVector3("viewPos", Camera::activeCam->transform->position);
        }
        if(!lights.empty() && lights[0]->transform->hasChanged) {
            opaques[i]->getShader()->setLight("light", lights[0]);
        }
        opaques[i]->render();
    }

    sortTransparents();

    for(size_t i = 0; i < transparents.size(); i++) {
        if(Camera::activeCam->transform->hasChanged) {
            transparents[i]->getShader()->setVector3("viewPos", Camera::activeCam->transform->position);
        }
        if(!lights.empty() && lights[0]->transform->hasChanged) {
            transparents[i]->getShader()->setLight("light", lights[0]);
        }
        transparents[i]->render();
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


void Scene::onModelCreated(MeshRenderer* _model) {
    if(_model->getShader()->isTransparent()) {
        transparents.push_back(_model);
    }
    else {
        opaques.push_back(_model);
    }
}

void Scene::onModelDestroyed(MeshRenderer* _model) {
    std::vector<MeshRenderer*>* modelList;
    if(_model->getShader()->isTransparent()) { modelList = &transparents; }
    else { modelList = &opaques; }

    for(size_t i = 0; i < modelList->size(); i++) {
        if(_model == modelList->at(i)) {
            modelList->erase(modelList->begin() + i);
            modelList->shrink_to_fit();
        }
    }
}