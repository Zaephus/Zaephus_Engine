
#pragma once

#include <ZMath.h>

class Bounds;
class GameObject;

class RayCast3D {
    public:
        bool canHitFromInside = false;

        Vector3 position = Vector3::zero();
        Vector3 direction = Vector3::back();

        RayCast3D(const Vector3& _pos, const Vector3& _dir);
        RayCast3D(const Vector3& _pos, const Vector3& _dir, bool _oneShot);

        [[nodiscard]] bool isColliding() const;
        [[nodiscard]] Bounds* getCollider() const;
        [[nodiscard]] Vector3 getCollisionPoint() const;

        void update();

    private:
        bool hasCollided = false;
        Bounds* hitCollider = nullptr;
        Vector3 collisionPoint = Vector3::zero();
};