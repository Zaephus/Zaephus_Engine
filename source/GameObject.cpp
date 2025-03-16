
#include "GameObject.h"

#include <algorithm>
#include <iostream>

#include <Vectors/Vector2.h>

#include "Component.h"
#include "RayCast3D.h"
#include "Scene.h"
#include "Transform.h"

Action<void(GameObject*)> GameObject::gameObjectCreatedCall = Action<void(GameObject*)>();
Action<void(GameObject*)> GameObject::gameObjectDestroyedCall = Action<void(GameObject*)>();

GameObject::GameObject() {
    Scene::startGameObjectCall.bind<GameObject, &GameObject::internalStart>(this);
    Scene::updateGameObjectCall.bind<GameObject, &GameObject::internalUpdate>(this);

    gameObjectCreatedCall.invoke(this);
}

GameObject::~GameObject() {
    delete transform;

    for(size_t i = 0; i < components.size(); i++) {
        delete components[i];
    }

}

void GameObject::destroy() {
    Scene::startGameObjectCall.unbind<GameObject, &GameObject::internalStart>(this);
    Scene::updateGameObjectCall.unbind<GameObject, &GameObject::internalUpdate>(this);

    gameObjectDestroyedCall.invoke(this);
}

void GameObject::internalStart() {
    Scene::startGameObjectCall.unbind<GameObject, &GameObject::internalStart>(this);

    start();

    for(size_t i = 0; i < components.size(); i++) {
        components[i]->start();
    }
}

void GameObject::internalUpdate() {
    update();

    for(size_t i = 0; i < components.size(); i++) {
        components[i]->update();
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