
#pragma once

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
        static Action<void()> startRenderItemCall;
        static Action<void()> updateRenderItemCall;

        Window* window = nullptr;

        bool hasFinishedSetup = false;

        ~Renderer();

        void initialize();
        void setRenderBuffer(RenderBuffer* _buffer);

        void setClearColor(float _r, float _g, float _b, float _a);
        void setClearColor(Color _c);

    private:
        Color clearColor = Color::white();
        bool clearColorChanged = true;

        RenderBuffer* renderBuffer = nullptr;

        std::vector<MeshRenderer> meshRenderers;
        std::vector<MultiMeshRenderer> multiMeshRenderers;
        std::vector<Light> lights;

        void handleSetup();
        void render();

        void changeClearColor();

        void transferRenderData();
        void clearScreen() const;
        void renderObjects() const;

        void sortMeshRenderers();
};