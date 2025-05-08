
#include "RenderBuffer.h"

#include "Light.h"
#include "MeshRenderer.h"
#include "MultiMeshRenderer.h"

void RenderBuffer::bind() {
    while(flag.test_and_set(std::memory_order_acquire)) {}
}

void RenderBuffer::unbind() {
    flag.clear(std::memory_order_release);
}

void RenderBuffer::setMeshRenderers(const std::vector<MeshRenderer*>& _meshRenderers) {
    meshRenderers.clear();
    meshRenderers.reserve(_meshRenderers.size());

    for(size_t i = 0; i < _meshRenderers.size(); i++) {
        meshRenderers[i] = *_meshRenderers[i];
    }
}

void RenderBuffer::setMultiMeshRenderers(const std::vector<MultiMeshRenderer*>& _multiMeshRenderers) {
    multiMeshRenderers.clear();
    multiMeshRenderers.reserve(_multiMeshRenderers.size());

    for(size_t i = 0; i < _multiMeshRenderers.size(); i++) {
        multiMeshRenderers[i] = *_multiMeshRenderers[i];
    }
}

void RenderBuffer::setLights(const std::vector<Light*>& _lights) {
    lights.clear();
    lights.reserve(_lights.size());

    for(size_t i = 0; i < _lights.size(); i++) {
        lights[i] = *_lights[i];
    }
}