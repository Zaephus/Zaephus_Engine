
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
#include "RenderBuffer.h"
#include "Shader.h"
#include "Transform.h"

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

Action<void()> Renderer::startRenderItemCall = Action<void()>();
Action<void()> Renderer::updateRenderItemCall = Action<void()>();

Renderer::~Renderer() {
    delete window;
}

void Renderer::initialize() {
#ifdef ENABLE_PROFILING
    tracy::SetThreadName("Render Thread");
#endif

    handleSetup();

    while(!window->shouldClose()) {
        render();
    }
}

void Renderer::setRenderBuffer(RenderBuffer* _buffer) {
    renderBuffer = _buffer;
}

void Renderer::setClearColor(float _r, float _g, float _b, float _a) { setClearColor({_r, _g, _b, _a}); }
void Renderer::setClearColor(const Color _c) {
    clearColor = _c;
    clearColorChanged = true;
}

void Renderer::handleSetup() {
    window = new Window();
    window->initialize(1200, 600, "Zaephus Engine");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    hasFinishedSetup = true;
}

void Renderer::render() {
#ifdef ENABLE_PROFILING
    ZoneScopedC(0x0062ff);
#endif

    if(clearColorChanged) {
        changeClearColor();
    }

    transferRenderData();
    sortMeshRenderers();

    clearScreen();

    renderObjects();

    window->presentFrame();
}

void Renderer::changeClearColor() {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    clearColorChanged = false;
}

void Renderer::transferRenderData() {
#ifdef ENABLE_PROFILING
    ZoneScopedC(0x0062ff);
#endif

    renderBuffer->bind();

    meshRenderers = renderBuffer->meshRenderers;
    multiMeshRenderers = renderBuffer->multiMeshRenderers;
    lights = renderBuffer->lights;

    renderBuffer->unbind();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void Renderer::clearScreen() const { // NOLINT(*-convert-member-functions-to-static)
#ifdef ENABLE_PROFILING
    ZoneScopedC(0x0062ff);
#endif

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderObjects() const {
#ifdef ENABLE_PROFILING
    ZoneScopedC(0x0062ff);
#endif

    for(size_t i = 0; i < meshRenderers.size(); i++) {
        meshRenderers[i].getShader()->setVector3("viewPos", Camera::activeCam->transform->position);
        meshRenderers[i].getShader()->setLight("light", &lights[0]);
        meshRenderers[i].render();
    }

    for(size_t i = 0; i < multiMeshRenderers.size(); i++) {
        multiMeshRenderers[i].getShader()->setVector3("viewPos", Camera::activeCam->transform->position);
        multiMeshRenderers[i].getShader()->setLight("light", &lights[0]);
        multiMeshRenderers[i].render();
    }
}

bool meshRendererCompare(const MeshRenderer& _a, const MeshRenderer& _b) {
    if(_a.getShader()->isTransparent() == true && _b.getShader()->isTransparent() == true) {
        return Vector3::distance(Camera::activeCam->transform->position, _a.transform->position)
             < Vector3::distance(Camera::activeCam->transform->position, _b.transform->position);
    }
    return !_a.getShader()->isTransparent();
}

void Renderer::sortMeshRenderers() {
#ifdef ENABLE_PROFILING
    ZoneScopedC(0x0062ff);
#endif

    std::ranges::sort(meshRenderers, meshRendererCompare);
}