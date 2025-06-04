
#include "Destructible.h"

#include <Action.h>

Destructible::Destructible(Action<void()>* _destroyCall) {
    destroyCall = _destroyCall;
}

void Destructible::markForDestruction() {
    destroyCall->bind<Destructible, &Destructible::internalDestroy>(this);
}

void Destructible::internalDestroy() {
    destroyCall->unbind<Destructible, &Destructible::internalDestroy>(this);
    destroy();
}