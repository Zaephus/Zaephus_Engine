
#include "Object.h"

#include "Action.h"
#include "Scene.h"

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

Object::Object() {
    Scene::startGameObjectCall.bind<Object, &Object::internalStart>(this);
    Scene::updateGameObjectCall.bind<Object, &Object::internalUpdate>(this);
}

Object::~Object() {
    Scene::startGameObjectCall.unbind<Object, &Object::internalStart>(this);
    Scene::updateGameObjectCall.unbind<Object, &Object::internalUpdate>(this);
}

const std::type_info& Object::getType() {
    return typeid(*this);
}

void Object::internalStart() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("start object", 0xff0000);
#endif

    Scene::startGameObjectCall.unbind<Object, &Object::internalStart>(this);

    start();
}

void Object::internalUpdate() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("update object", 0xff0000);
#endif

    update();
}