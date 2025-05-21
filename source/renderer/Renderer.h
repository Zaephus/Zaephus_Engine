
#pragma once

#include <atomic>
#include <vector>

#include "Color.h"

template <typename T>
class Action;

class Light;
class MeshRenderer;
class MultiMeshRenderer;
class Window;
class RenderBuffer;

class Renderer {
    public:
        static Action<void()> initRenderItemCall;
        static Action<void()> destroyRenderItemCall;

        Window* window = nullptr;

        bool hasFinishedSetup = false;

        void initialize();

        bool isInitialized() const;
        bool testAndSetReadyForRender();

        void setClearColor(float _r, float _g, float _b, float _a);
        void setClearColor(Color _c);

    private:
        Color clearColor = Color::white();
        bool clearColorChanged = true;

        std::atomic<bool> initFlag = false;
        std::atomic<bool> readyForRenderFlag = false;

        std::vector<Light*> lights;
        std::vector<MeshRenderer*> meshRenderers;
        std::vector<MultiMeshRenderer*> multiMeshRenderers;

        void handleSetup();
        void handleExit();
        void render();

        void changeClearColor();

        void clearScreen() const;
        void renderObjects() const;

        void sortMeshRenderers();

        void onLightCreated(Light* _light);
        void onLightDestroyed(Light* _light);

        void onMeshRendererCreated(MeshRenderer* _renderer);
        void onMultiMeshRendererCreated(MultiMeshRenderer* _renderer);
};