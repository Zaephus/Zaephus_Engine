
#pragma once

#include <vector>

#include <ZMath.h>

class Bounds;
class GameObject;

class RayCast3D {
    public:
        bool canHitFromInside = false;

        Vector3 position = Vector3::zero();
        Vector3 direction = Vector3::back();

        RayCast3D(const Vector3& _pos, const Vector3& _dir);
        ~RayCast3D();

        [[nodiscard]] bool isColliding() const;
        [[nodiscard]] Bounds* getCollider() const;
        [[nodiscard]] Vector3 getCollisionPoint() const;

        static void update(const std::vector<GameObject*>& _gameObjects);

    private:
        bool hasCollided = false;
        Bounds* hitCollider = nullptr;
        Vector3 collisionPoint = Vector3::zero();

        void resetHitInfo();

        static std::vector<RayCast3D*> rays;
};