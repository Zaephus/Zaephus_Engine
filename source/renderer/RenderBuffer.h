
#pragma once

#include <atomic>
#include <vector>

class Light;
class MeshRenderer;
class MultiMeshRenderer;

class RenderBuffer {
    public:
        std::vector<MeshRenderer> meshRenderers;
        std::vector<MultiMeshRenderer> multiMeshRenderers;
        std::vector<Light> lights;

        void bind();
        void unbind();

        void setMeshRenderers(const std::vector<MeshRenderer*>& _meshRenderers);
        void setMultiMeshRenderers(const std::vector<MultiMeshRenderer*>& _multiMeshRenderers);
        void setLights(const std::vector<Light*>& _lights);

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
};