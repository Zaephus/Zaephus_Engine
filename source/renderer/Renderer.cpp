
#include "Renderer.h"

#include <algorithm>
#include <iostream>

#include <glad/gl.h>

#include "Action.h"
#include "Camera.h"
#include "Light.h"
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

    while(!window->shouldClose()) {
        render();
    }

    handleExit();
}

bool Renderer::isInitialized() const {
    return initFlag;
}

bool Renderer::testAndSetReadyForRender() {
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
    Light::lightCreatedCall.bind<Renderer, &Renderer::onLightCreated>(this);
    Light::lightDestroyedCall.bind<Renderer, &Renderer::onLightDestroyed>(this);

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
    destroyRenderItemCall.invoke();

    lights.clear();
    meshRenderers.clear();
    multiMeshRenderers.clear();

    Light::lightCreatedCall.unbind<Renderer, &Renderer::onLightCreated>(this);
    Light::lightDestroyedCall.unbind<Renderer, &Renderer::onLightDestroyed>(this);

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

    for(size_t i = 0; i < meshRenderers.size(); i++) {
        meshRenderers[i]->getShader()->setVector3("viewPos", Camera::activeCam->transform->position);
        meshRenderers[i]->getShader()->setLight("light", lights[0]);
        meshRenderers[i]->render();
    }

    for(size_t i = 0; i < multiMeshRenderers.size(); i++) {
        multiMeshRenderers[i]->getShader()->setVector3("viewPos", Camera::activeCam->transform->position);
        multiMeshRenderers[i]->getShader()->setLight("light", lights[0]);
        multiMeshRenderers[i]->render();
    }
}

bool meshRendererCompare(const MeshRenderer* _a, const MeshRenderer* _b) {
    if(_a->getShader()->isTransparent() == true && _b->getShader()->isTransparent() == true) {
        return Vector3::distance(Camera::activeCam->transform->position, _a->transform->position)
             > Vector3::distance(Camera::activeCam->transform->position, _b->transform->position);
    }
    return !_a->getShader()->isTransparent();
}

void Renderer::sortMeshRenderers() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Renderer::SortMeshRenderers", 0x0062ff);
#endif

    std::ranges::sort(meshRenderers, meshRendererCompare);
}

void Renderer::onLightCreated(Light* _light) {
    lights.push_back(_light);
}

void Renderer::onLightDestroyed(Light* _light) {
    for(size_t i = 0; i < lights.size(); i++) {
        if(_light == lights[i]) {
            lights.erase(lights.begin() + i);
            lights.shrink_to_fit();
        }
    }
}

void Renderer::onMeshRendererCreated(MeshRenderer* _renderer) {
    meshRenderers.push_back(_renderer);
}

void Renderer::onMultiMeshRendererCreated(MultiMeshRenderer* _renderer) {
    multiMeshRenderers.push_back(_renderer);
}