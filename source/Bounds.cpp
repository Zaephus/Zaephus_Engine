
#include "Bounds.h"

#include <iostream>

#include <ZMath.h>

#include <Transform.h>

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
    return {right, top, front};
}
Vector3 Bounds::getTopRightBack() const {
    return {right, top, back};
}
Vector3 Bounds::getTopLeftFront() const {
    return {left, top, front};
}
Vector3 Bounds::getTopLeftBack() const {
    return {left, top, back};
}

Vector3 Bounds::getBottomRightFront() const {
    return {right, bottom, front};
}
Vector3 Bounds::getBottomRightBack() const {
    return {right, bottom, back};
}
Vector3 Bounds::getBottomLeftFront() const {
    return {left, bottom, front};
}
Vector3 Bounds::getBottomLeftBack() const {
    return {left, bottom, back};
}

Vector3 Bounds::getCenter() const {
    return {
        (right + left) / 2,
        (bottom + top) / 2,
        (front + back) / 2
    };
}

bool Bounds::containsPoint(const Vector3& _position) const {
    return left <= _position.x && _position.x <= right
        && top <= _position.y && _position.y <= bottom
        && front <= _position.z && _position.z <= back;
}

bool Bounds::intersectsLine(const Vector3& _worldPos, const Vector3& _dir, Vector3& _hit) const {
    const Vector3 pos = transform->objectMatrix().inverse() * _worldPos;
    const Vector3 dir = (transform->directionMatrix().inverse() * _dir.normalized()).normalized();

    float x, y, z;

    // right-left check
    if(dir.y == 0.0f && dir.z == 0.0f) {
        y = pos.y;
        z = pos.z;
        if(intersectsXPlane(y, z)) {
            std::cout << "Intersect on X" <<std::endl;
            _hit = transform->objectMatrix() * Vector3(pos.x, y, z);
            return true;
        }
    }
    else if(dir.x != 0.0f) {
        y = pos.y + dir.y * (right - pos.x) / dir.x;
        z = pos.z + dir.z * (right - pos.x) / dir.x;
        if(intersectsXPlane(y, z)) {
            std::cout << "Intersect on X" <<std::endl;
            _hit = transform->objectMatrix() * Vector3(right, y, z);
            return true;
        }

        y = pos.y + dir.y * (left - pos.x) / dir.x;
        z = pos.z + dir.z * (left - pos.x) / dir.x;
        if(intersectsXPlane(y, z)) {
            std::cout << "Intersect on X" <<std::endl;
            _hit = transform->objectMatrix() * Vector3(left, y, z);
            return true;
        }
    }

    // top-bottom check
    if(dir.x == 0.0f && dir.z == 0.0f) {
        x = pos.x;
        z = pos.z;
        if(intersectsYPlane(x, z)) {
            std::cout << "Intersect on Y" << std::endl;
            _hit = transform->objectMatrix() * Vector3(x, pos.y, z);
            return true;
        }
    }
    else if(dir.y != 0.0f) {
        x = pos.x + dir.x * (top - pos.y) / dir.y;
        z = pos.z + dir.z * (top - pos.y) / dir.y;
        if(intersectsYPlane(x, z)) {
            std::cout << "Intersect on Y" << std::endl;
            _hit = transform->objectMatrix() * Vector3(x, top, z);
            return true;
        }

        x = pos.x + dir.x * (bottom - pos.y) / dir.y;
        z = pos.z + dir.z * (bottom - pos.y) / dir.y;
        if(intersectsYPlane(x, z)) {
            std::cout << "Intersect on Y" << std::endl;
            _hit = transform->objectMatrix() * Vector3(x, bottom, z);
            return true;
        }
    }

    // front-back check
    if(dir.x == 0.0f && dir.y == 0.0f) {
        x = pos.x;
        y = pos.y;
        if(intersectsZPlane(x, y)) {
            std::cout << "Intersect on Z" << std::endl;
            _hit = transform->objectMatrix() * Vector3(x, y, pos.z);
            return true;
        }
    }
    else if(dir.z != 0.0f) {
        x = pos.x + dir.x * (front - pos.z) / dir.z;
        y = pos.y + dir.y * (front - pos.z) / dir.z;
        if(intersectsZPlane(x, y)) {
            std::cout << "Intersect on Z" << std::endl;
            _hit = transform->objectMatrix() * Vector3(x, y, front);
            return true;
        }

        x = pos.x + dir.x * (back - pos.z) / dir.z;
        y = pos.y + dir.y * (back - pos.z) / dir.z;
        if(intersectsZPlane(x, y)) {
            std::cout << "Intersect on Z" << std::endl;
            _hit = transform->objectMatrix() * Vector3(x, y, back);
            return true;
        }
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