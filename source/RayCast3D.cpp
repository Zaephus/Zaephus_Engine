
#include "RayCast3D.h"

#include <Bounds.h>
#include <GameObject.h>

RayCast3D::RayCast3D(const Vector3& _pos, const Vector3& _dir) {
    position = _pos;
    direction = _dir;

    rays.push_back(this);
}

RayCast3D::~RayCast3D() {
    for(int i = rays.size()-1; i >= 0; --i) {
        if(rays[i] == this) {
            rays.erase(rays.begin() + i);
            rays.shrink_to_fit();
            return;
        }
    }
}

bool RayCast3D::isColliding() const { return hasCollided; }
Bounds* RayCast3D::getCollider() const { return hitCollider; }
Vector3 RayCast3D::getCollisionPoint() const { return collisionPoint; }

void RayCast3D::update(const std::vector<GameObject*>& _gameObjects) {
    for(RayCast3D* ray : rays) {
        ray->resetHitInfo();
        for(GameObject* gameObject : _gameObjects) {
            const Bounds* bounds = gameObject->getComponent<Bounds>();
            if(bounds == nullptr) { continue; }
            if(bounds->containsPoint(ray->position)) { continue; }

        }
    }
}

void RayCast3D::resetHitInfo() {
    hasCollided = false;
    hitCollider = nullptr;
    collisionPoint = Vector3::zero();
}