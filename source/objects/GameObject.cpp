
#include "GameObject.h"

#include <algorithm>
#include <iostream>

#include "Action.h"
#include "Component.h"
#include "Transform.h"

Action<void(GameObject*)> GameObject::gameObjectCreatedCall = Action<void(GameObject*)>();
Action<void(GameObject*)> GameObject::gameObjectDestroyedCall = Action<void(GameObject*)>();

GameObject::GameObject() {
    transform = new Transform();

    gameObjectCreatedCall.invoke(this);
}

GameObject::~GameObject() {
    delete transform;

    for(size_t i = 0; i < components.size(); i++) {
        delete components[i];
    }
}

void GameObject::destroy() {
    gameObjectDestroyedCall.invoke(this);
}

void GameObject::addComponent(Component* _component) {
    _component->gameObject = this;
    _component->transform = transform;

    components.push_back(_component);
}

void GameObject::removeComponent(const Component* _component) {
    const auto it = std::ranges::find(components.begin(), components.end(), _component);
    if(it == components.end()) {
        std::cout << "Component does not exist in game object." << std::endl;
        return;
    }

    components.erase(it);
}