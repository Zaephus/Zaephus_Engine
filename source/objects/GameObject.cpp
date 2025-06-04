
#include "GameObject.h"

#include <algorithm>
#include <iostream>

#include "Action.h"
#include "Component.h"
#include "Scene.h"
#include "Transform.h"

Action<void(GameObject*)> GameObject::gameObjectCreatedCall = Action<void(GameObject*)>();
Action<void(GameObject*)> GameObject::gameObjectDestroyedCall = Action<void(GameObject*)>();

GameObject::GameObject() : Destructible(&Scene::destroyObjectCall) {
    transform = new Transform();

    gameObjectCreatedCall.invoke(this);
}

void GameObject::markForDestruction() {
    Destructible::markForDestruction();

    for(size_t i = 0; i < components.size(); i++) {
        components[i]->markForDestruction();
    }
}

void GameObject::destroy() {
    delete transform;

    gameObjectDestroyedCall.invoke(this);
}

void GameObject::addComponent(Component* _component) {
    _component->gameObject = this;
    _component->transform = transform;

    components.push_back(_component);
}

void GameObject::removeComponent(Component* _component) {
    const auto it = std::ranges::find(components.begin(), components.end(), _component);
    if(it == components.end()) {
        std::cout << "Component does not exist in game object." << std::endl;
        return;
    }

    _component->markForDestruction();
    components.erase(it);
}