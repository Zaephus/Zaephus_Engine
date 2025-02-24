
#include "GameObject.h"

#include <algorithm>
#include <iostream>

#include <Vectors/Vector2.h>

#include "Bounds.h"
#include "BoundsRenderer.h"
#include "Camera.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "Transform.h"
#include "Window.h"

Action<void(GameObject*)> GameObject::gameObjectCreatedCall = Action<void(GameObject*)>();
bool GameObject::renderBounds = false;

GameObject::GameObject() {
    transform = new Transform();
    bounds = new Bounds();

    addComponent(bounds);

    Scene::startGameObjectCall.bind<GameObject, &GameObject::internalStart>(this);
    Scene::updateGameObjectCall.bind<GameObject, &GameObject::internalUpdate>(this);

    Window::mousePressedCall.bind<GameObject, &GameObject::onMousePressed>(this);
    gameObjectCreatedCall.invoke(this);
}

GameObject::~GameObject() {
    delete transform;

    for(size_t i = 0; i < components.size(); i++) {
        delete components[i];
    }
}

void GameObject::internalStart() {
    if(renderBounds) {
        BoundsRenderer* boundsRenderer = new BoundsRenderer(bounds);
        addComponent(boundsRenderer);
    }
    start();

    for(Component* c : components) {
        c->start();
    }
}

void GameObject::internalUpdate() {
    if(renderBounds) {
        getComponent<BoundsRenderer>()->render();
    }
    update();

    for(Component* c : components) {
        c->update();
    }
}

void GameObject::addComponent(Component* _component) {
    _component->gameObject = this;
    _component->transform = transform;

    components.push_back(_component);
}

void GameObject::removeComponent(const Component* _component) {
    const auto it = std::ranges::find(components.begin(), components.end(), _component);
    if(it == components.end()) {
        std::cout << "Component does not exist in gameobject." << std::endl;
        return;
    }

    components.erase(it);
}

// ReSharper disable once CppPassValueParameterByConstReference
void GameObject::onMousePressed(const int _button, const Vector2 _pos) { // NOLINT(*-unnecessary-value-param)
    const Vector2 clipPos = Window::activeWindow->screenToClip(_pos);
    const Vector3 viewPos = Camera::activeCam->projectionMatrix.inverse() * Vector3(clipPos.x, clipPos.y, 0.0f);
    const Vector3 worldPos = Camera::activeCam->viewMatrix().inverse() * viewPos;

    const Vector3 localPos = transform->objectMatrix().inverse() * worldPos;
    const Vector3 localDir = localPos - transform->objectMatrix().inverse() * Camera::activeCam->transform->position;

    if(bounds->intersectsLine(localPos, localDir)) {
        clicked(_button);
    }
}