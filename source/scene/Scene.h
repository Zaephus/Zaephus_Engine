
#pragma once

#include <vector>

#include "Action.h"

class Window;
class GameObject;
class MeshRenderer;
class MultiMeshRenderer;
class PointLight;
class Renderer;

struct RenderBuffer;

class Scene {
    public:
        static Scene* activeScene;

        static Action<void()> startObjectCall;
        static Action<void()> updateObjectCall;

        Action<void()> notifyEndOfFrame = Action<void()>();

        std::vector<GameObject*> gameObjects;

        Renderer* renderer = nullptr;

        Scene();
        virtual ~Scene() = default;

        void initialize();

        [[nodiscard]] Window* getWindow() const;

    protected:
        virtual void start() = 0;
        virtual void update() = 0;

        bool shouldRenderAxis = false;

    private:
        std::vector<GameObject*> gameObjectsToDestroy;

        void handleSetup();
        void handleExit();

        void internalStart();
        void internalUpdate();

        void handleDestroyingGameObjects();

        void setupAxis();

        void onGameObjectCreated(GameObject* _gameObject);
        void onGameObjectDestroyed(GameObject* _gameObject);
};