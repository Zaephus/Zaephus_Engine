
#include "Camera.h"

#include <iostream>

#include <ZMath.h>

#include "Action.h"
#include "Color.h"
#include "Transform.h"
#include "Window.h"
#include "RayCast3D.h"

Camera* Camera::activeCam = nullptr;

Camera::Camera() {
    Window::sizeChangedCall.bind<Camera, &Camera::onWindowResized>(this);

    if(activeCam == nullptr) {
        activeCam = this;
    }
}

Camera::~Camera() {
    Window::sizeChangedCall.unbind<Camera, &Camera::onWindowResized>(this);
}

void Camera::update() {
    if(lastProjMatrix != projectionMatrix) { projectionChanged = true; }
    else { projectionChanged = false; }

    lastProjMatrix = projectionMatrix;
}

Matrix4x4 Camera::viewMatrix() const {
    const Matrix4x4 t = Matrix4x4::translateMatrix(transform->position).inverse();
    const Matrix4x4 r = Matrix4x4::rotateMatrix(transform->rotation).inverse();
    const Matrix4x4 view = r * t;

    return view;
}

void Camera::setAsActive() {
    activeCam = this;
}

void Camera::onWindowResized(const int _w, const int _h) {
    aspect = static_cast<float>(_w) / static_cast<float>(_h);
    if(projectionType == ProjectionType::perspective) {
        projectionMatrix = perspectiveMatrix(fovY, aspect, near, far);
    }
    else if(projectionType == ProjectionType::orthographic) {
        projectionMatrix = orthographicMatrix(size, aspect, near, far);
    }
}

Vector3 Camera::screenToWorldPos(const Vector2& _screenPos) const {
    const Vector2 clipPos = Window::activeWindow->screenToClip(_screenPos);
    const Vector3 viewPos = projectionMatrix.inverse() * Vector3(clipPos.x, clipPos.y, 0.0f);
    const Vector3 nearPlanePos = viewMatrix().inverse() * viewPos;
    const Vector3 direction = (transform->position - nearPlanePos).normalized();

    const float angle = Vector3::angle(Vector3::up(), direction);
    const float depth = transform->position.y / std::cos(angle);

    return transform->position - direction * depth;
}

Vector3 Camera::screenToWorldPos(const Vector2& _screenPos, const float _depth) const {
    const Vector2 clipPos = Window::activeWindow->screenToClip(_screenPos);
    const Vector3 viewPos = projectionMatrix.inverse() * Vector3(clipPos.x, clipPos.y, 0.0f);
    const Vector3 nearPlanePos = viewMatrix().inverse() * viewPos;
    const Vector3 direction = (transform->position - nearPlanePos).normalized();
    return transform->position - direction * _depth;
}

RayCast3D Camera::screenToRay(const Vector2& _screenPos) const {
    return screenToRay(_screenPos, false);
}

RayCast3D Camera::screenToRay(const Vector2& _screenPos, const bool _oneShot) const {
    const Vector3 pos = screenToWorldPos(_screenPos, 1.0f);
    const Vector3 dir = pos - transform->position;

    return { pos, dir, _oneShot };
}

Camera* Camera::createPerspectiveCamera(const float _fovY, const float _near, const float _far) {
    const Vector2Int windowSize = Window::activeWindow->getSize();
    const float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

    Camera* cam = new Camera();
    cam->fovY = _fovY;
    cam->aspect = aspect;
    cam->near = _near;
    cam->far = _far;
    cam->projectionType = ProjectionType::perspective;

    cam->projectionMatrix = perspectiveMatrix(_fovY, aspect, _near, _far);

    return cam;
}

Camera* Camera::createOrthographicCamera(const float _sizeY, const float _near, const float _far) {
    const Vector2Int windowSize = Window::activeWindow->getSize();
    const float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

    Camera* cam = new Camera();
    cam->size = _sizeY;
    cam->aspect = aspect;
    cam->near = _near;
    cam->far = _far;
    cam->projectionType = ProjectionType::orthographic;

    cam->projectionMatrix = orthographicMatrix(_sizeY, aspect, _near, _far);

    return cam;
}

Matrix4x4 Camera::perspectiveMatrix(const float _fovY, const float _aspect, const float _near, const float _far) {
    if(_fovY <= 0.0f || _fovY > ZMath::pi) {
        std::cerr << "Fov out of range: " << _fovY << std::endl;
    }
    if(_aspect <= 0) {
        std::cerr << "Aspect ratio not positive: " << _aspect << std::endl;
    }
    if(_far <= 0) {
        std::cerr << "Far plane not positive: " << _far << std::endl;
    }
    if(_near <= 0 || _near >= _far) {
        std::cerr << "Near plane out of range: " << _near << std::endl;
    }

    const float t = _near * std::tan(0.5f * _fovY);
    const float r = _aspect * t;

    const float a = _near / r;
    const float b = _near / t;

    const float c = -(_far + _near) / (_far - _near);
    const float d = -(2.0f * _far * _near) / (_far - _near);

    return {
        a,    0.0f, 0.0f,  0.0f,
        0.0f, b,    0.0f,  0.0f,
        0.0f, 0.0f, c,     d,
        0.0f, 0.0f, -1.0f, 0.0f
    };
}

Matrix4x4 Camera::orthographicMatrix(const float _sizeY, const float _aspect, const float _near, const float _far) {
    const float t = _sizeY / 2.0f;
    const float r = _aspect * t;

    const float a = 1.0f / r;
    const float b = 1.0f / t;

    const float c = -2.0f / (_far - _near);
    const float d = -(_far + _near) / (_far - _near);

    return {
        a,    0.0f, 0.0f, 0.0f,
        0.0f, b,    0.0f, 0.0f,
        0.0f, 0.0f, c,    d,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}