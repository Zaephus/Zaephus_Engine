
#include "Bounds.h"

#include <GameObject.h>
#include <iostream>

#include <ZMath.h>

#include <Transform.h>

#include "BoundsRenderer.h"

bool Bounds::shouldRender = false;

Bounds::Bounds() : Bounds({ 0.5f, 0.5f, 0.5f }) {}
Bounds::Bounds(const Vector3& _scale) {
    right = _scale.x;
    left = -_scale.x;

    top = _scale.y;
    bottom = -_scale.y;

    front = _scale.z;
    back = -_scale.z;
}

void Bounds::start() {
    if(shouldRender) {
        gameObject->addComponent(new BoundsRenderer(this));
    }
}

void Bounds::update() {
    if(shouldRender) {
        gameObject->getComponent<BoundsRenderer>()->render();
    }
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
    Vector3 hitPos;
    bool hasHit = false;

    float x, y, z;

    // right-left check
    if(dir.y == 0.0f && dir.z == 0.0f) {
        y = pos.y;
        z = pos.z;
        if(intersectsPlanesOnX(y, z)) {
            const Vector3 newHit = { pos.x, y, z };

            if(!hasHit) { hitPos = newHit; }
            else { calculateHit(hitPos, newHit, pos); }

            hasHit = true;
        }
    }
    else if(dir.x != 0.0f) {
        y = pos.y + dir.y * (right - pos.x) / dir.x;
        z = pos.z + dir.z * (right - pos.x) / dir.x;
        if(intersectsPlanesOnX(y, z)) {
            const Vector3 newHit = { right, y, z };

            if(!hasHit) { hitPos = newHit; }
            else { calculateHit(hitPos, newHit, pos); }

            hasHit = true;
        }

        y = pos.y + dir.y * (left - pos.x) / dir.x;
        z = pos.z + dir.z * (left - pos.x) / dir.x;
        if(intersectsPlanesOnX(y, z)) {
            const Vector3 newHit = { left, y, z};

            if(!hasHit) { hitPos = newHit; }
            else { calculateHit(hitPos, newHit, pos); }

            hasHit = true;
        }
    }

    // top-bottom check
    if(dir.x == 0.0f && dir.z == 0.0f) {
        x = pos.x;
        z = pos.z;
        if(intersectsPlanesOnY(x, z)) {
            const Vector3 newHit = { x, pos.y, z };

            if(!hasHit) { hitPos = newHit; }
            else { calculateHit(hitPos, newHit, pos); }

            hasHit = true;
        }
    }
    else if(dir.y != 0.0f) {
        x = pos.x + dir.x * (top - pos.y) / dir.y;
        z = pos.z + dir.z * (top - pos.y) / dir.y;
        if(intersectsPlanesOnY(x, z)) {
            const Vector3 newHit = { x, top, z };

            if(!hasHit) { hitPos = newHit; }
            else { calculateHit(hitPos, newHit, pos); }

            hasHit = true;
        }

        x = pos.x + dir.x * (bottom - pos.y) / dir.y;
        z = pos.z + dir.z * (bottom - pos.y) / dir.y;
        if(intersectsPlanesOnY(x, z)) {
            const Vector3 newHit = { x, bottom, z };

            if(!hasHit) { hitPos = newHit; }
            else { calculateHit(hitPos, newHit, pos); }

            hasHit = true;
        }
    }

    // front-back check
    if(dir.x == 0.0f && dir.y == 0.0f) {
        x = pos.x;
        y = pos.y;
        if(intersectsPlanesOnZ(x, y)) {
            const Vector3 newHit = { x, y, pos.z };

            if(!hasHit) { hitPos = newHit; }
            else { calculateHit(hitPos, newHit, pos); }

            hasHit = true;
        }
    }
    else if(dir.z != 0.0f) {
        x = pos.x + dir.x * (front - pos.z) / dir.z;
        y = pos.y + dir.y * (front - pos.z) / dir.z;
        if(intersectsPlanesOnZ(x, y)) {
            const Vector3 newHit = { x, y, front };

            if(!hasHit) { hitPos = newHit; }
            else { calculateHit(hitPos, newHit, pos); }

            hasHit = true;
        }

        x = pos.x + dir.x * (back - pos.z) / dir.z;
        y = pos.y + dir.y * (back - pos.z) / dir.z;
        if(intersectsPlanesOnZ(x, y)) {
            const Vector3 newHit = { x, y, back };

            if(!hasHit) { hitPos = newHit; }
            else { calculateHit(hitPos, newHit, pos); }

            hasHit = true;
        }
    }

    _hit = transform->objectMatrix() * hitPos;
    return hasHit;
}

void Bounds::calculateHit(Vector3& _hit, const Vector3& _newHit, const Vector3& _rayOrigin) {
    if(Vector3::distance(_hit, _rayOrigin) >= Vector3::distance(_newHit, _rayOrigin)) {
        _hit = _newHit;
    }
}

bool Bounds::intersectsPlanesOnX(const float _y, const float _z) const {
    return bottom <= _y && _y <= top && back <= _z && _z <= front;
}
bool Bounds::intersectsPlanesOnY(const float _x, const float _z) const {
    return left <= _x && _x <= right && back <= _z && _z <= front;
}
bool Bounds::intersectsPlanesOnZ(const float _x, const float _y) const {
    return left <= _x && _x <= right && bottom <= _y && _y <= top;
}