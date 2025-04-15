
#include "Transform.h"

#include <format>
#include <iostream>
#include <ZMath.h>

Transform::Transform() {
    position = Vector3::zero();
    rotation = Quaternion::identity();
    scale = Vector3::one();
}
Transform::Transform(const Vector3 &_p, const Quaternion &_r, const Vector3 &_s) {
    position = _p;
    rotation = _r;
    scale = _s;
}

void Transform::update() {
    if(lastPos != position || lastRot != rotation || lastScale != scale) {
        hasChanged = true;
    }
    else {
        hasChanged = false;
    }

    lastPos = position;
    lastRot = rotation;
    lastScale = scale;
}

std::string Transform::toString() const {
    return std::format("Position: {}\nRotation: {}\nScale: {}", position.toString(), rotation.toString(), scale.toString());
}

void Transform::rotate(const Vector3& _eulerAngles) { rotate(_eulerAngles.x, _eulerAngles.y, _eulerAngles.z); }
void Transform::rotate(const float _xDeg, const float _yDeg, const float _zDeg) {
    rotate(Quaternion::fromEuler(0.0f, 0.0f, _zDeg));
    rotate(Quaternion::fromEuler(0.0f, _yDeg, 0.0f));
    rotate(Quaternion::fromEuler(_xDeg, 0.0f, 0.0f));
//    rotate(Quaternion::fromAxisAngle(forward(), _zDeg * Math::deg2rad));
//    rotate(Quaternion::fromAxisAngle(up(), _yDeg * Math::deg2rad));
//    rotate(Quaternion::fromAxisAngle(right(), _xDeg * Math::deg2rad));
}
void Transform::rotate(const Quaternion& _q) {
    rotation = rotation * _q;
}

void Transform::rotateAround(const Vector3& _point, const Vector3 &_axis, float _deg) {
    rotateAround(_point, _axis, _deg, false);
}
void Transform::rotateAround(const Vector3 &_point, const Vector3& _axis, float _deg, bool _lookAtAxis) {
    const Quaternion rot = Quaternion::fromAxisAngle(_axis, _deg * ZMath::deg2rad);
    position = rot * (position - _point) + _point;

    if(_lookAtAxis) { rotation *= rot; }
}

void Transform::lookAt(const Vector3& _point) {
    Vector3 directional = (position - _point).normalized();
    Vector3 perpendicular = Vector3::cross(directional, Vector3::up());
    Vector3 up = Vector3::cross(directional, perpendicular);

    lookAt(_point, up);
}
void Transform::lookAt(const Vector3& _point, const Vector3& _up) {
    std::cerr << "Not yet implemented." << std::endl;
}

Vector3 Transform::right() const {
    return (rotation * Vector3::right()).normalized();
}
Vector3 Transform::forward() const {
    return (rotation * Vector3::forward()).normalized();
}
Vector3 Transform::up() const {
    return (rotation * Vector3::up()).normalized();
}

Matrix4x4 Transform::objectMatrix() const {
    const Matrix4x4 t = Matrix4x4::translateMatrix(position);
    const Matrix4x4 r = Matrix4x4::rotateMatrix(rotation);
    const Matrix4x4 s = Matrix4x4::scaleMatrix(scale);
    Matrix4x4 result;

    if(parent == nullptr) { result = t * r * s; }
    else { result = parent->objectMatrix() * t * r * s; }
    return result;
}

Matrix4x4 Transform::directionMatrix() const {
    const Matrix4x4 r = Matrix4x4::rotateMatrix(rotation);
    const Matrix4x4 s = Matrix4x4::scaleMatrix(scale);
    Matrix4x4 result;

    if(parent == nullptr) { result = r * s; }
    else { result = parent->directionMatrix() * r * s; }
    return result;
}