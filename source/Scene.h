
#pragma once

#include <Action.h>
#include <vector>

class Window;
class GameObject;
class MeshRenderer;
class Light;

class Scene {
    public:
        Window* window = nullptr;

        Scene() = default;
        virtual ~Scene() = 0;

        void initialize();

        static Action<void()> startGameObjectCall;
        static Action<void()> updateGameObjectCall;

    protected:
        virtual void start() = 0;
        virtual void update() = 0;

        bool shouldRenderAxis = false;

    private:
        std::vector<GameObject*> gameObjects;

        std::vector<Light*> lights;
        std::vector<MeshRenderer*> opaques;
        std::vector<MeshRenderer*> transparents;

        MeshRenderer* xLine = nullptr;
        MeshRenderer* xCube = nullptr;

        MeshRenderer* yLine = nullptr;
        MeshRenderer* yCube = nullptr;

        MeshRenderer* zLine = nullptr;
        MeshRenderer* zCube = nullptr;

        void handleSetup();
        void internalStart();
        void internalUpdate();

        void setupAxis();
        void setupLights() const;

        void sortTransparents();

        void render();

        void onGameObjectCreated(GameObject* _gameObject);
        void onLightCreated(Light* _light);
        void onModelCreated(MeshRenderer* _model);
};