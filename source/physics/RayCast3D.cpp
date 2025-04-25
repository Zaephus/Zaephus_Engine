
#include "RayCast3D.h"

#include <ZMath.h>

#include "Bounds.h"
#include "GameObject.h"
#include "Scene.h"

RayCast3D::RayCast3D(const Vector3& _pos, const Vector3& _dir) : RayCast3D(_pos, _dir, false) {}
RayCast3D::RayCast3D(const Vector3& _pos, const Vector3& _dir, const bool _oneShot) {
    position = _pos;
    direction = _dir;

    if(_oneShot) { update(); }
}

bool RayCast3D::isColliding() const { return hasCollided; }
Bounds* RayCast3D::getCollider() const { return hitCollider; }
Vector3 RayCast3D::getCollisionPoint() const { return collisionPoint; }

void RayCast3D::update() {
    bool hasHit = false;
    Bounds* hitBounds = nullptr;
    Vector3 hitPos;

    for(GameObject* gameObject : Scene::activeScene->gameObjects) {
        Bounds* bounds = gameObject->getComponent<Bounds>();
        if(bounds == nullptr) { continue; }
        if(bounds->containsPoint(position)) { continue; }

        Vector3 hit;
        if(bounds->intersectsLine(position, direction, hit)) {
            if(hasHit) {
                if(Vector3::distance(hit, position) > Vector3::distance(hitPos, position)) {
                    continue;
                }
            }

            hasHit = true;
            hitBounds = bounds;
            hitPos = hit;
        }
    }

    hasCollided = hasHit;
    hitCollider = hitBounds;
    collisionPoint = hitPos;
}