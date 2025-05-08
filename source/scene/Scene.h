
#pragma once

#include <vector>

template <typename T>
class Action;

class Window;
class GameObject;
class MeshRenderer;
class MultiMeshRenderer;
class Light;
class Renderer;

struct RenderBuffer;

class Scene {
    public:
        static Scene* activeScene;

        static Action<void()> startObjectCall;
        static Action<void()> updateObjectCall;

        std::vector<GameObject*> gameObjects;

        Renderer* renderer = nullptr;

        Scene();
        virtual ~Scene() = 0;

        void initialize();

        Window* getWindow() const;

    protected:
        virtual void start() = 0;
        virtual void update() = 0;

        bool shouldRenderAxis = false;

    private:
        std::vector<GameObject*> gameObjectsToDestroy;

        std::vector<Light*> lights;
        std::vector<MeshRenderer*> meshRenderers;
        std::vector<MultiMeshRenderer*> multiMeshRenderers;

        RenderBuffer* renderBuffer = nullptr;

        void handleSetup();
        void internalStart();
        void internalUpdate();

        void transferRenderData() const;

        void handleDestroyingGameObjects();

        void setupAxis();

        void onGameObjectCreated(GameObject* _gameObject);
        void onGameObjectDestroyed(GameObject* _gameObject);

        void onLightCreated(Light* _light);
        void onLightDestroyed(Light* _light);

        void onMeshRendererCreated(MeshRenderer* _renderer);
        void onMeshRendererDestroyed(MeshRenderer* _renderer);

        void onMultiMeshRendererCreated(MultiMeshRenderer* _renderer);
        void onMultiMeshRendererDestroyed(MultiMeshRenderer* _renderer);
};