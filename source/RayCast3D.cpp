
#include "RayCast3D.h"

#include <Bounds.h>

bool RayCast3D::isColliding() const { return hasCollided; }
Bounds* RayCast3D::getCollider() const { return hitCollider; }
Vector3 RayCast3D::getCollisionPoint() const { return collisionPoint; }

void RayCast3D::update() {

}