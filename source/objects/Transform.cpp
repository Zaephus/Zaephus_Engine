
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

std::string Transform::toString() const {
    return std::format("Position: {}\nRotation: {}\nScale: {}", position.toString(), rotation.toString(), scale.toString());
}

void Transform::rotate(const Vector3& _eulerAngles) { rotate(_eulerAngles.x, _eulerAngles.y, _eulerAngles.z); }
void Transform::rotate(const float _xDeg, const float _yDeg, const float _zDeg) {
    rotate(Quaternion::fromEuler(_xDeg, _yDeg, _zDeg));
}
void Transform::rotate(const Quaternion& _q) {
    rotation = _q * rotation;
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
    const Vector3 dir = (position - _point).normalized();
    const Vector3 fwd = forward();

    const float dot = Vector3::dot(dir, fwd);
    if(dot >= 0.9999) { return; }

    Vector3 up = Vector3::up();
    if(dot > -0.9999) {
        up = Vector3::cross(dir, fwd);
    }

    const float angle = Vector3::angle(fwd, dir);

    const Quaternion rot = Quaternion::fromAxisAngle(-up, ZMath::deg2rad * angle);
    rotate(rot);
}

void Transform::lookAt(const Vector3& _point, const Vector3& _up) {
    std::cerr << "Not yet implemented." << std::endl;
}

Vector3 Transform::globalPosition() const {
    if(parent != nullptr) {
        return parent->objectMatrix() * position;
    }
    return position;
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