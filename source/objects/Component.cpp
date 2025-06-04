
#include "Component.h"

#include "Scene.h"

Component::Component() : Destructible(&Scene::destroyObjectCall) {}

Component::Component(Action<void()>* _destroyCall) : Destructible(_destroyCall) {}