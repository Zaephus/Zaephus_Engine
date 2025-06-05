
#pragma once

#include <atomic>
#include <vector>

#include "Color.h"

class DirectionalLight;
template <typename T>
class Action;

class PointLight;
class MeshRenderer;
class MultiMeshRenderer;
class Shader;
class Window;
class RenderBuffer;

class Renderer {
    public:
        static Action<void()> initRenderObjectCall;
        static Action<void()> destroyRenderObjectCall;
        static Action<void()> cleanupRenderObjectsCall;

        Window* window = nullptr;

        bool hasFinishedSetup = false;
        bool shouldExit = false;

        void initialize();

        [[nodiscard]] bool isInitialized() const;
        bool isReadyForRender();
        bool testAndSetReadyForRender();
        bool isSorting();

        void setReadyForCleanup();

        void setClearColor(float _r, float _g, float _b, float _a);
        void setClearColor(Color _c);

    private:
        Color clearColor = Color::black();
        bool clearColorChanged = true;

        std::atomic<bool> initFlag = false;
        std::atomic<bool> readyForRenderFlag = false;
        std::atomic<bool> canStartCleanupFlag = false;
        std::atomic<bool> duringSortFlag = false;

        std::vector<DirectionalLight*> dirLights;
        std::vector<PointLight*> pointLights;

        std::vector<MeshRenderer*> meshRenderers;
        std::vector<MultiMeshRenderer*> multiMeshRenderers;

        void handleSetup();
        void handleExit();
        void render();

        void changeClearColor();

        void clearScreen() const;
        void renderObjects() const;

        void setShaderData(Shader* _shader) const;

        void sortMeshRenderers();

        void onPointLightCreated(PointLight* _light);
        void onPointLightDestroyed(PointLight* _light);

        void onDirLightCreated(DirectionalLight* _light);
        void onDirLightDestroyed(DirectionalLight* _light);

        void onMeshRendererCreated(MeshRenderer* _renderer);
        void onMeshRendererDestroyed(MeshRenderer* _renderer);

        void onMultiMeshRendererCreated(MultiMeshRenderer* _renderer);
        void onMultiMeshRendererDestroyed(MultiMeshRenderer* _renderer);
};