
#include "GameObject.h"

#include <iostream>

#include <Vectors/Vector2.h>

#include "Bounds.h"
#include "BoundsRenderer.h"
#include "Camera.h"
#include "Color.h"
#include "Component.h"
#include "Transform.h"
#include "Window.h"
#include "Graphics/MeshRenderer.h"
#include "Graphics/Shader.h"

Action<void(GameObject*)> GameObject::gameObjectCreatedCall = Action<void(GameObject*)>();
bool GameObject::renderBounds = false;

GameObject::GameObject() {
    transform = new Transform();
    bounds = new Bounds();

    addComponent(bounds);

    if(renderBounds) {
        BoundsRenderer* boundsRenderer = new BoundsRenderer(bounds);
        addComponent(boundsRenderer);
    }

    Window::mousePressedCall.bind<GameObject, &GameObject::onMousePressed>(this);
    gameObjectCreatedCall.invoke(this);
}

GameObject::~GameObject() {
    delete transform;
    std::cout << "Transform was deleted" << std::endl;
}

void GameObject::update() {
    if(renderBounds) {
        getComponent<BoundsRenderer>()->render();
    }
}


void GameObject::addComponent(Component* _component) {
    _component->gameObject = this;
    _component->transform = transform;

    const std::type_info& typeInfo = _component->getType();
    std::cout << typeInfo.name() << std::endl;

    components.push_back(_component);
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