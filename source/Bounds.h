
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

        void start() override;
        void update() override;

        [[nodiscard]] Vector3 getTopRightFront() const;
        [[nodiscard]] Vector3 getTopRightBack() const;
        [[nodiscard]] Vector3 getTopLeftFront() const;
        [[nodiscard]] Vector3 getTopLeftBack() const;

        [[nodiscard]] Vector3 getBottomRightFront() const;
        [[nodiscard]] Vector3 getBottomRightBack() const;
        [[nodiscard]] Vector3 getBottomLeftFront() const;
        [[nodiscard]] Vector3 getBottomLeftBack() const;

        [[nodiscard]] Vector3 getCenter() const;

        [[nodiscard]] bool containsPoint(const Vector3& _position) const;
        [[nodiscard]] bool intersectsLine(const Vector3& _worldPos, const Vector3& _dir, Vector3& _hit) const;

        static bool shouldRender;

    private:
        [[nodiscard]] bool intersectsPlanesOnX(float _y, float _z) const;
        [[nodiscard]] bool intersectsPlanesOnY(float _x, float _z) const;
        [[nodiscard]] bool intersectsPlanesOnZ(float _x, float _y) const;

        static void calculateHit(Vector3& _hit, const Vector3& _newHit, const Vector3& _rayOrigin);
};