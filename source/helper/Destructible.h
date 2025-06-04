
#pragma once

template <typename T>
class Action;

class Destructible {
    public:
        explicit Destructible(Action<void()>* _destroyCall);

        virtual void markForDestruction();

    protected:
        virtual ~Destructible() {}

        virtual void destroy() {}

    private:
        Action<void()>* destroyCall;

        void internalDestroy();
};