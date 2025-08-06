
#include "DebugCameraController.h"

#include <iostream>

#include <ZMath.h>

#include "Input.h"
#include "TimeUtils.h"
#include "Transform.h"

void DebugCameraController::update() {
    move();

    if(Input::isMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
        if(mouseDown == false) {
            Input::setMouseMode(GLFW_CURSOR_DISABLED);
        }
        rotate();
        mouseDown = true;
    }
    else if(Input::isMouseUp(GLFW_MOUSE_BUTTON_LEFT)) {
        if(mouseDown == true) {
            Input::setMouseMode(GLFW_CURSOR_NORMAL);
        }
        mouseDown = false;
    }
}

void DebugCameraController::move() const {
    if(Input::isKeyDown(GLFW_KEY_W)) {
        transform->position -= transform->forward() * moveSpeed * Time::deltaTime;
    }
    if(Input::isKeyDown(GLFW_KEY_S)) {
        transform->position += transform->forward() * moveSpeed * Time::deltaTime;
    }
    if(Input::isKeyDown(GLFW_KEY_A)) {
        transform->position -= transform->right() * moveSpeed * Time::deltaTime;
    }
    if(Input::isKeyDown(GLFW_KEY_D)) {
        transform->position += transform->right() * moveSpeed * Time::deltaTime;
    }
    if(Input::isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        transform->position -= Vector3::up() * moveSpeed * Time::deltaTime;
    }
    if(Input::isKeyDown(GLFW_KEY_SPACE)) {
        transform->position += Vector3::up() * moveSpeed * Time::deltaTime;
    }
}

void DebugCameraController::rotate() {
    Vector2 mouseDelta = Input::getMouseDelta();

    if(mouseDown == false) {
        mouseDelta = Vector2::zero();
    }

    yaw -= mouseDelta.x * sensitivity;
    pitch -= mouseDelta.y * sensitivity;

    if(pitch > 89.0f) { pitch = 89.0f; }
    if(pitch < -89.0f) { pitch = -89.0f; }

    transform->rotation = Quaternion::fromEuler({ pitch, yaw, 0.0f });
}