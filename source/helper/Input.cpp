
#include "Input.h"

#include <ZMath.h>

#include "Action.h"
#include "Window.h"

Window* Input::window = nullptr;
Vector2 Input::mousePos = Vector2::zero();
Vector2 Input::lastMousePos = Vector2::zero();

void Input::initialize() {
    Window::mousePressedCall.bind<&Input::onMousePressed>();

    window = Window::activeWindow;

    glfwSetCursorPosCallback(window->glfwWindow, onMouseMoved);
}

void Input::dispose() {
    Window::mousePressedCall.unbind<&Input::onMousePressed>();
}

bool Input::isKeyDown(const int _key) {
    return glfwGetKey(window->glfwWindow, _key) == GLFW_PRESS;
}

bool Input::isKeyUp(const int _key) {
    return glfwGetKey(window->glfwWindow, _key) == GLFW_RELEASE;
}

bool Input::isMouseDown(const int _button) {
    return glfwGetMouseButton(window->glfwWindow, _button) == GLFW_PRESS;
}

bool Input::isMouseUp(const int _button) {
    return glfwGetMouseButton(window->glfwWindow, _button) == GLFW_RELEASE;
}

void Input::setMouseMode(const int _mode) {
    window->requestInputModeChange(GLFW_CURSOR, _mode);
}

Vector2 Input::getMousePosition() {
    double x = 0;
    double y = 0;

    glfwGetCursorPos(window->glfwWindow, &x, &y);
    Vector2 pos = { static_cast<float>(x), static_cast<float>(y) };
    return pos;
}

Vector2 Input::getMouseDelta() {
    const Vector2 delta = mousePos - lastMousePos;
    lastMousePos = mousePos;

    return delta;
}

void Input::onMousePressed(const int _button, const Vector2 _pos) {
}

void Input::onMouseMoved(GLFWwindow* window, const double x, const double y) {
    mousePos = { static_cast<float>(x), static_cast<float>(y) };
}