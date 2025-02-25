
#include "Scene.h"

#include <iterator>
#include <map>

#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "TimeUtils.h"
#include "Transform.h"
#include "Window.h"

Action<void()> Scene::startGameObjectCall = Action<void()>();
Action<void()> Scene::updateGameObjectCall = Action<void()>();

Scene::~Scene() {
    delete window;

    for(const GameObject* gameObject : gameObjects) { delete gameObject; }
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

    GameObject::gameObjectCreatedCall.bind<Scene, &Scene::onGameObjectCreated>(this);
    Light::lightCreatedCall.bind<Scene, &Scene::onLightCreated>(this);
    MeshRenderer::modelCreatedCall.bind<Scene, &Scene::onModelCreated>(this);
}

void Scene::internalStart() {
    if(shouldRenderAxis) { setupAxis(); }
    setupLights();

    startGameObjectCall.invoke();

    sortTransparents();
}

void Scene::internalUpdate() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateGameObjectCall.invoke();
    update();
    render();
    Time::tick();

    window->presentFrame();
}

void Scene::setupAxis() {
    xLine = MeshRenderer::loadModel(MeshRenderer::cube);
    xLine->setOverrideShader(Shader::unlitShader(Color::red()));
    xLine->transform->scale = { 100.0f, 0.01f, 0.01f };

    xCube = MeshRenderer::loadModel(MeshRenderer::cube);
    xCube->setOverrideShader(Shader::unlitShader(Color::red()));
    xCube->transform->position = { 1.0f, 0.0f, 0.0f };
    xCube->transform->scale = { 0.05f, 0.05f, 0.05f };

    yLine = MeshRenderer::loadModel(MeshRenderer::cube);
    yLine->setOverrideShader(Shader::unlitShader(Color::green()));
    yLine->transform->scale = { 0.01f, 100.0f, 0.01f };

    yCube = MeshRenderer::loadModel(MeshRenderer::cube);
    yCube->setOverrideShader(Shader::unlitShader(Color::green()));
    yCube->transform->position = { 0.0f, 1.0f, 0.0f };
    yCube->transform->scale = { 0.05f, 0.05f, 0.05f };

    zLine = MeshRenderer::loadModel(MeshRenderer::cube);
    zLine->setOverrideShader(Shader::unlitShader(Color::blue()));
    zLine->transform->scale = { 0.01f, 0.01f, 100.0f };

    zCube = MeshRenderer::loadModel(MeshRenderer::cube);
    zCube->setOverrideShader(Shader::unlitShader(Color::blue()));
    zCube->transform->position = { 0.0f, 0.0f, 1.0f };
    zCube->transform->scale = { 0.05f, 0.05f, 0.05f };
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
    for(size_t i = 0; i < opaques.size(); i++) {
        opaques[i]->setVector3("viewPos", Camera::activeCam->transform->position);
        if(!lights.empty()) { opaques[i]->setLight("light", lights[0]); }
        opaques[i]->render();
    }

    sortTransparents();

    for(size_t i = 0; i < transparents.size(); i++) {
        transparents[i]->setVector3("viewPos", Camera::activeCam->transform->position);
        if(!lights.empty()) { transparents[i]->setLight("light", lights[0]); }
        transparents[i]->render();
    }
}

void Scene::onGameObjectCreated(GameObject* _gameObject) {
    gameObjects.push_back(_gameObject);
}

void Scene::onLightCreated(Light* _light) {
    lights.push_back(_light);
}

void Scene::onModelCreated(MeshRenderer* _model) {
    if(_model->isTransparent()) {
        transparents.push_back(_model);
    }
    else {
        opaques.push_back(_model);
    }
}