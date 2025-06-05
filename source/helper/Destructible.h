
#pragma once

#include "Action.h"

class Destructible {
    public:
        Action<void()> notifyDestroyed = Action<void()>();
        explicit Destructible(Action<void()>* _destroyCall);

        virtual void markForDestruction();

    protected:
        virtual ~Destructible() {}

        virtual void destroy() {}

    private:
        Action<void()>* destroyCall;

        void internalDestroy();
};