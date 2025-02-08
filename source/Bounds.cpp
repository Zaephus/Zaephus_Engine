
#include "Bounds.h"

#include <Vectors/Vector3.h>

Bounds::Bounds() {
    right = 0.5f;
    left = -0.5f;

    top = 0.5f;
    bottom = -0.5f;

    front = 0.5f;
    back = -0.5f;
}

Bounds::Bounds(const Vector3& _scale) {
    right = _scale.x;
    left = -_scale.y;

    top = _scale.y;
    bottom = -_scale.y;

    front = _scale.z;
    back = -_scale.z;
}

Vector3 Bounds::getTopRightFront() const {
    return Vector3(right, top, front);
}
Vector3 Bounds::getTopRightBack() const {
    return Vector3(right, top, back);
}
Vector3 Bounds::getTopLeftFront() const {
    return Vector3(left, top, front);
}
Vector3 Bounds::getTopLeftBack() const {
    return Vector3(left, top, back);
}

Vector3 Bounds::getBottomRightFront() const {
    return Vector3(right, bottom, front);
}
Vector3 Bounds::getBottomRightBack() const {
    return Vector3(right, bottom, back);
}
Vector3 Bounds::getBottomLeftFront() const {
    return Vector3(left, bottom, front);
}
Vector3 Bounds::getBottomLeftBack() const {
    return Vector3(left, bottom, back);
}

bool Bounds::containsPoint(const Vector3& _position) const {
    return left <= _position.x && _position.x <= right
        && top <= _position.y && _position.y <= bottom
        && front <= _position.z && _position.z <= back;
}

bool Bounds::intersectsLine(const Vector3& _startingPoint, const Vector3& _direction) const {
    float x, y, z;
    // front check
    if(_direction.x == 0.0f) {
        y = _startingPoint.y;
        z = _startingPoint.z;
        if(intersectsXPlane(y, z)) { return true; }
    }
    else {
        y = _startingPoint.y + _direction.y * (right - _startingPoint.x) / _direction.x;
        z = _startingPoint.z + _direction.z * (right - _startingPoint.x) / _direction.x;
        if(intersectsXPlane(y, z)) { return true; }

        y = _startingPoint.y + _direction.y * (left - _startingPoint.x) / _direction.x;
        z = _startingPoint.z + _direction.z * (left - _startingPoint.x) / _direction.x;
        if(intersectsXPlane(y, z)) { return true; }
    }

    if(_direction.y == 0.0f) {
        x = _startingPoint.x;
        z = _startingPoint.z;
        if(intersectsYPlane(x, z)) { return true; }
    }
    else {
        x = _startingPoint.x + _direction.x * (top - _startingPoint.y) / _direction.y;
        z = _startingPoint.z + _direction.z * (top - _startingPoint.y) / _direction.y;
        if(intersectsYPlane(x, z)) { return true; }

        x = _startingPoint.x + _direction.x * (bottom - _startingPoint.y) / _direction.y;
        z = _startingPoint.z + _direction.z * (bottom - _startingPoint.y) / _direction.y;
        if(intersectsYPlane(x, z)) { return true; }
    }

    if(_direction.z == 0.0f) {
        x = _startingPoint.x;
        y = _startingPoint.y;
        if(intersectsZPlane(x, y)) { return true; }
    }
    else {
        x = _startingPoint.x + _direction.x * (front - _startingPoint.z) / _direction.z;
        y = _startingPoint.y + _direction.y * (front - _startingPoint.z) / _direction.z;
        if(intersectsZPlane(x, y)) { return true; }

        x = _startingPoint.x + _direction.x * (back - _startingPoint.z) / _direction.z;
        y = _startingPoint.y + _direction.y * (back - _startingPoint.z) / _direction.z;
        if(intersectsZPlane(x, y)) { return true; }
    }

    return false;
}

bool Bounds::intersectsXPlane(const float _y, const float _z) const {
    return bottom <= _y && _y <= top && back <= _z && _z <= front;
}
bool Bounds::intersectsYPlane(const float _x, const float _z) const {
    return left <= _x && _x <= right && back <= _z && _z <= front;
}
bool Bounds::intersectsZPlane(const float _x, const float _y) const {
    return left <= _x && _x <= right && bottom <= _y && _y <= top;
}