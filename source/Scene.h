
#pragma once

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
        virtual void start() = 0;
        virtual void update() = 0;

    protected:
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

        void setupAxis();
        void setupLights() const;

        void sortTransparents();

        void startGameObjects() const;
        void updateGameObjects() const;
        void render();

        void onGameObjectCreated(GameObject* _gameObject);
        void onLightCreated(Light* _light);
        void onModelCreated(MeshRenderer* _model);
};