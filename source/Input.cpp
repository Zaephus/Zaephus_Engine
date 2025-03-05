
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

bool Input::isMouseDown(const int _button) {
    return glfwGetMouseButton(Window::activeWindow->window, _button) == GLFW_PRESS;
}

bool Input::isMouseUp(const int _button) {
    return glfwGetMouseButton(Window::activeWindow->window, _button) == GLFW_RELEASE;
}

Vector2 Input::getMousePosition() {
    double x = 0;
    double y = 0;

    glfwGetCursorPos(Window::activeWindow->window, &x, &y);
    Vector2 pos = { static_cast<float>(x), static_cast<float>(y) };
    return pos;
}

void Input::onMousePressed(const int _button, const Vector2 _pos) {
    RayCast3D ray = Camera::activeCam->screenToRay(_pos, true);

    if(ray.isColliding()) {
        ray.getCollider()->gameObject->clicked(_button, ray.getCollisionPoint());
    }
}