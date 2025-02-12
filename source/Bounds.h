
#pragma once

#include "Component.h"

struct Matrix4x4;
struct Vector3;

class Bounds : public Component {
    public:
        float right;
        float left;
        float top;
        float bottom;
        float front;
        float back;

        Bounds();
        explicit Bounds(const Vector3& _scale);

        Vector3 getTopRightFront() const;
        Vector3 getTopRightBack() const;
        Vector3 getTopLeftFront() const;
        Vector3 getTopLeftBack() const;

        Vector3 getBottomRightFront() const;
        Vector3 getBottomRightBack() const;
        Vector3 getBottomLeftFront() const;
        Vector3 getBottomLeftBack() const;

        Vector3 getCenter() const;

        bool containsPoint(const Vector3& _position) const;
        bool intersectsLine(const Vector3& _startingPoint, const Vector3& _direction) const;

    private:
        bool intersectsXPlane(float _y, float _z) const;
        bool intersectsYPlane(float _x, float _z) const;
        bool intersectsZPlane(float _x, float _y) const;
};