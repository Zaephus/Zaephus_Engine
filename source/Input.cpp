
#include "Input.h"

#include <ZMath.h>

#include <Bounds.h>
#include <Camera.h>
#include <RayCast3D.h>
#include <Window.h>

void Input::initialize() {
    Window::mousePressedCall.bind<&Input::onMousePressed>();
}

bool Input::isKeyDown(const int _key) {
    return glfwGetKey(Window::activeWindow->window, _key) == GLFW_PRESS;
}

void Input::onMousePressed(const int _button, const Vector2 _pos) {
    const Vector3 pos = Camera::activeCam->screenToWorldPos(_pos);
    const Vector3 dir = pos - Camera::activeCam->transform->position;

    RayCast3D ray = RayCast3D(pos, dir, true);

    if(ray.isColliding()) {
        ray.getCollider()->gameObject->clicked(_button, ray.getCollisionPoint());
    }
}