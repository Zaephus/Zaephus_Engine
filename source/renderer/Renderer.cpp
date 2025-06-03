
#include "Renderer.h"

#include <algorithm>
#include <format>
#include <iostream>

#include <glad/gl.h>

#include "Action.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "MeshRenderer.h"
#include "MultiMeshRenderer.h"
#include "Window.h"
#include "Shader.h"
#include "Transform.h"

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

Action<void()> Renderer::initRenderItemCall = Action<void()>();
Action<void()> Renderer::destroyRenderItemCall = Action<void()>();

void Renderer::initialize() {
    initFlag = false;

#ifdef ENABLE_PROFILING
    tracy::SetThreadName("Render Thread");
#endif

    handleSetup();

    while(!shouldExit) {
        render();
    }

    handleExit();
}

bool Renderer::isInitialized() const {
    return initFlag;
}

bool Renderer::testAndSetReadyForRender() {
    if(shouldExit) { return true; }

    const bool val = readyForRenderFlag;

    if(readyForRenderFlag) { readyForRenderFlag = false; }

    return val;
}

void Renderer::setClearColor(float _r, float _g, float _b, float _a) { setClearColor({_r, _g, _b, _a}); }
void Renderer::setClearColor(const Color _c) {
    clearColor = _c;
    clearColorChanged = true;
}

void Renderer::handleSetup() {
    DirectionalLight::dirLightCreatedCall.bind<Renderer, &Renderer::onDirLightCreated>(this);
    DirectionalLight::dirLightDestroyedCall.bind<Renderer, &Renderer::onDirLightDestroyed>(this);

    PointLight::pointLightCreatedCall.bind<Renderer, &Renderer::onPointLightCreated>(this);
    PointLight::pointLightDestroyedCall.bind<Renderer, &Renderer::onPointLightDestroyed>(this);

    MeshRenderer::meshRendererCreatedCall.bind<Renderer, &Renderer::onMeshRendererCreated>(this);
    MultiMeshRenderer::multiMeshRendererCreatedCall.bind<Renderer, &Renderer::onMultiMeshRendererCreated>(this);

    window = new Window();
    window->initialize(1200, 600, "Zaephus Engine");

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initFlag = true;
}

void Renderer::handleExit() {
    readyForRenderFlag = true;

    destroyRenderItemCall.invoke();

    pointLights.clear();
    meshRenderers.clear();
    multiMeshRenderers.clear();

    DirectionalLight::dirLightCreatedCall.unbind<Renderer, &Renderer::onDirLightCreated>(this);
    DirectionalLight::dirLightDestroyedCall.unbind<Renderer, &Renderer::onDirLightDestroyed>(this);

    PointLight::pointLightCreatedCall.unbind<Renderer, &Renderer::onPointLightCreated>(this);
    PointLight::pointLightDestroyedCall.unbind<Renderer, &Renderer::onPointLightDestroyed>(this);

    MeshRenderer::meshRendererCreatedCall.unbind<Renderer, &Renderer::onMeshRendererCreated>(this);
    MultiMeshRenderer::multiMeshRendererCreatedCall.unbind<Renderer, &Renderer::onMultiMeshRendererCreated>(this);

    delete window;
}

void Renderer::render() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Renderer::Render", 0x0062ff);
#endif

    while(readyForRenderFlag) {}

    initRenderItemCall.invoke();

    if(clearColorChanged) {
        changeClearColor();
    }

    sortMeshRenderers();

    clearScreen();

    renderObjects();

    window->processCallStack();
    window->presentFrame();

    shouldExit = window->shouldClose();

    readyForRenderFlag = true;
}

void Renderer::changeClearColor() {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    clearColorChanged = false;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void Renderer::clearScreen() const { // NOLINT(*-convert-member-functions-to-static)
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Renderer::ClearScreen", 0x0062ff);
#endif

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderObjects() const {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Renderer::RenderObjects", 0x0062ff);
#endif

    for(size_t i = 0; i < pointLights.size(); i++) {
        pointLights[i]->calculateAttenuation();
    }

    for(size_t i = 0; i < meshRenderers.size(); i++) {
        setShaderData(meshRenderers[i]->getShader());
        meshRenderers[i]->render();
    }

    for(size_t i = 0; i < multiMeshRenderers.size(); i++) {
        setShaderData(multiMeshRenderers[i]->getShader());
        multiMeshRenderers[i]->render();
    }
}

void Renderer::setShaderData(Shader* _shader) const {
    _shader->setVector3("viewPos", Camera::activeCam->transform->position);

    _shader->setInt("dirLightAmount", static_cast<int>(dirLights.size()));
    for(size_t i = 0; i < dirLights.size(); i++) {
        std::string name = std::format("dirLights[{}]", i);
        _shader->setDirLight(name, dirLights[i]);
    }

    _shader->setInt("pointLightAmount", static_cast<int>(pointLights.size()));
    for(size_t i = 0; i < pointLights.size(); i++) {
        std::string name = std::format("pointLights[{}]", i);
        _shader->setPointLight(name, pointLights[i]);
    }
}

bool meshRendererCompare(const MeshRenderer* _a, const MeshRenderer* _b) {
    return Vector3::distance(Camera::activeCam->transform->position, _a->transform->position)
         > Vector3::distance(Camera::activeCam->transform->position, _b->transform->position);
}

void Renderer::sortMeshRenderers() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Renderer::SortMeshRenderers", 0x0062ff);
#endif

    std::ranges::sort(meshRenderers, meshRendererCompare);
}

void Renderer::onPointLightCreated(PointLight* _light) {
    pointLights.push_back(_light);
}

void Renderer::onPointLightDestroyed(PointLight* _light) {
    for(size_t i = 0; i < pointLights.size(); i++) {
        if(_light == pointLights[i]) {
            pointLights.erase(pointLights.begin() + i);
            pointLights.shrink_to_fit();
        }
    }
}

void Renderer::onDirLightCreated(DirectionalLight* _light) {
    dirLights.push_back(_light);
}

void Renderer::onDirLightDestroyed(DirectionalLight* _light) {
    for(size_t i = 0; i < dirLights.size(); i++) {
        if(_light == dirLights[i]) {
            dirLights.erase(dirLights.begin() + i);
            dirLights.shrink_to_fit();
        }
    }
}

void Renderer::onMeshRendererCreated(MeshRenderer* _renderer) {
    meshRenderers.push_back(_renderer);
}

void Renderer::onMultiMeshRendererCreated(MultiMeshRenderer* _renderer) {
    multiMeshRenderers.push_back(_renderer);
}