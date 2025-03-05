
#include "Camera.h"

#include <iostream>
#include <glad/gl.h>

#include <ZMath.h>

#include "Color.h"
#include "TimeUtils.h"
#include "Transform.h"
#include "Window.h"
#include "RayCast3D.h"

Camera* Camera::activeCam = nullptr;

Camera::Camera() {
    Window::sizeChangedCall.bind<Camera, &Camera::onWindowResized>(this);
    Window::keyPressedCall.bind<Camera, &Camera::onKeyPressed>(this);
    Window::cursorMovedCall.bind<Camera, &Camera::onCursorMovement>(this);

    if(activeCam == nullptr) {
        activeCam = this;
    }
}

Matrix4x4 Camera::viewMatrix() const {
    const Matrix4x4 t = Matrix4x4::translateMatrix(transform->position).inverse();
    const Matrix4x4 r = Matrix4x4::rotateMatrix(transform->rotation).inverse();
    const Matrix4x4 view = r * t;

    return view;
}

void Camera::setClearColor(float _r, float _g, float _b, float _a) { setClearColor({_r, _g, _b, _a}); }
void Camera::setClearColor(Color _c) {
    glClearColor(_c.r, _c.g, _c.b, _c.a);
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
    return viewMatrix().inverse() * viewPos;
}

RayCast3D Camera::screenToRay(const Vector2& _screenPos) const {
    return screenToRay(_screenPos, false);
}

RayCast3D Camera::screenToRay(const Vector2& _screenPos, const bool _oneShot) const {
    const Vector3 pos = screenToWorldPos(_screenPos);
    const Vector3 dir = pos - transform->position;

    return { pos, dir, _oneShot };
}

void Camera::onKeyPressed(int _key, int _action) {
    // if(_action == GLFW_PRESS) {
    //     switch(_key) {
    //         case GLFW_KEY_W:
    //             transform->position -= Time::deltaTime * speed * transform->forward();
    //             break;
    //         case GLFW_KEY_S:
    //             transform->position += Time::deltaTime * speed * transform->forward();
    //             break;
    //         case GLFW_KEY_D:
    //             transform->position += Time::deltaTime * speed * transform->right();
    //             break;
    //         case GLFW_KEY_A:
    //             transform->position -= Time::deltaTime * speed * transform->right();
    //             break;
    //         case GLFW_KEY_SPACE:
    //             transform->position += Time::deltaTime * speed * Vector3::up();
    //             break;
    //         case GLFW_KEY_LEFT_SHIFT:
    //             transform->position -= Time::deltaTime * speed * Vector3::up();
    //             break;
    //         default:
    //             break;
    //     }
    // }
}

void Camera::onCursorMovement(const Vector2 _mouseDelta) { // NOLINT(*-unnecessary-value-param)
//     Vector2 camRot = {
//         _mouseDelta.x * mouseSensitivity * Time::deltaTime,
//         _mouseDelta.y * mouseSensitivity * Time::deltaTime
//     };
//
// //    if(abs(Vector3::dot(transform->forward(), Vector3::up())) >= 0.9f ) {
// //        camRot.x = 0;
// //    }
//
//     Vector3 oldRot = transform->rotation.toEuler();
//     transform->rotation = Quaternion::identity();
//     transform->rotate(0.0f, oldRot.y - camRot.x, 0.0f);
//     transform->rotate(oldRot.x - camRot.y, 0.0f, 0.0f);
//
// //    transform->rotate(-camRot.y, -camRot.x, 0.0f);
// //    transform->rotate(Quaternion::fromAxisAngle(Vector3::up(), camRot.y * Math::deg2rad));
// //    transform->rotate(Quaternion::fromAxisAngle(transform->right(), camRot.x * Math::deg2rad));
// //    transform->rotation *= Quaternion::fromEuler(camRot.x, camRot.y, 0.0f);
// //    std::cout << "Current camera rotation: " << transform->rotation.toEuler().toString() << std::endl;
}

Camera* Camera::createPerspectiveCamera(const float _fovY, const float _aspect, const float _near, const float _far) {
    Camera* cam = new Camera();
    cam->fovY = _fovY;
    cam->aspect = _aspect;
    cam->near = _near;
    cam->far = _far;
    cam->projectionType = ProjectionType::perspective;

    cam->projectionMatrix = perspectiveMatrix(_fovY, _aspect, _near, _far);

    return cam;
}

Camera* Camera::createOrthographicCamera(const float _sizeY, const float _aspect, const float _near, const float _far) {
    Camera* cam = new Camera();
    cam->size = _sizeY;
    cam->aspect = _aspect;
    cam->near = _near;
    cam->far = _far;
    cam->projectionType = ProjectionType::orthographic;

    cam->projectionMatrix = orthographicMatrix(_sizeY, _aspect, _near, _far);

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