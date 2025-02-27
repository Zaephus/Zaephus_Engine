
#pragma once

#include <ZMath.h>

#include <GameObject.h>

class Bounds;

class RayCast3D : public GameObject {
    public:
        bool canHitFromInside = false;
        Vector3 direction = Vector3::back();

        bool isColliding() const;
        Bounds* getCollider() const;
        Vector3 getCollisionPoint() const;

    protected:
        void update() override;

    private:
        bool hasCollided = false;
        Bounds* hitCollider = nullptr;
        Vector3 collisionPoint = Vector3::zero();
};