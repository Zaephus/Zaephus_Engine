
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
Action<void(GameObject*)> GameObject::gameObjectDestroyedCall = Action<void(GameObject*)>();
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
    Scene::startGameObjectCall.unbind<GameObject, &GameObject::internalStart>(this);
    Scene::updateGameObjectCall.unbind<GameObject, &GameObject::internalUpdate>(this);

    Window::mousePressedCall.unbind<GameObject, &GameObject::onMousePressed>(this);

    delete transform;

    for(size_t i = 0; i < components.size(); i++) {
        delete components[i];
    }

    gameObjectDestroyedCall.invoke(this);
}

void GameObject::internalStart() {
    Scene::startGameObjectCall.unbind<GameObject, &GameObject::internalStart>(this);

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
    const Vector3 pos = Camera::activeCam->screenToWorldPos(_pos);
    const Vector3 dir = pos - Camera::activeCam->transform->position;

    Vector3 hit = Vector3::zero();

    if(bounds->intersectsLine(pos, dir, hit)) {
        std::cout << hit.toString() << std::endl;
        clicked(_button);
    }
}