
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
    meshRenderers.resize(_meshRenderers.size());

    for(size_t i = 0; i < _meshRenderers.size(); i++) {
        meshRenderers[i] = *_meshRenderers[i];
    }
}

void RenderBuffer::setMultiMeshRenderers(const std::vector<MultiMeshRenderer*>& _multiMeshRenderers) {
    multiMeshRenderers = std::vector<MultiMeshRenderer>(_multiMeshRenderers.size());

    for(size_t i = 0; i < _multiMeshRenderers.size(); i++) {
        memcpy(&multiMeshRenderers[i], _multiMeshRenderers[i], sizeof(MultiMeshRenderer));
        // multiMeshRenderers[i] = *_multiMeshRenderers[i];
    }
}

void RenderBuffer::setLights(const std::vector<Light*>& _lights) {
    lights.clear();
    lights.resize(_lights.size());

    for(size_t i = 0; i < _lights.size(); i++) {
        lights[i] = *_lights[i];
    }
}