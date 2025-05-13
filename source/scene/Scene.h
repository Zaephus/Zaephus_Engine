
#pragma once

#include <thread>
#include <vector>

template <typename T>
class Action;

class Window;
class GameObject;
class MeshRenderer;
class MultiMeshRenderer;
class Light;

class Scene {
    public:
        Window* window = nullptr;
        std::vector<GameObject*> gameObjects;

        static Scene* activeScene;

        Scene();
        virtual ~Scene() = 0;

        void initialize();

        static Action<void()> startGameObjectCall;
        static Action<void()> updateGameObjectCall;

    protected:
        virtual void start() = 0;
        virtual void update() = 0;

        bool shouldRenderAxis = false;

    private:
        std::vector<GameObject*> gameObjectsToDestroy;

        std::vector<Light*> lights;
        std::vector<MeshRenderer*> opaques;
        std::vector<MeshRenderer*> transparents;

        std::vector<MultiMeshRenderer*> multiOpaques;

        void handleSetup();
        void internalStart();
        void internalUpdate();

        void handleDestroyingGameObjects();

        void setupAxis();
        void setupLights() const;

        void sortTransparents();

        void render();

        void onGameObjectCreated(GameObject* _gameObject);
        void onGameObjectDestroyed(GameObject* _gameObject);

        void onLightCreated(Light* _light);
        void onLightDestroyed(Light* _light);

        void onMeshRendererCreated(MeshRenderer* _renderer);
        void onMeshRendererDestroyed(MeshRenderer* _renderer);

        void onMultiMeshRendererCreated(MultiMeshRenderer* _renderer);
        void onMultiMeshRendererDestroyed(MultiMeshRenderer* _renderer);
};