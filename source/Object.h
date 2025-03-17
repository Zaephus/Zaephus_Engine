
#pragma once

#include <typeinfo>

class Object {
    public:
        Object();
        virtual ~Object();

        virtual const std::type_info& getType();

    protected:
        virtual void start() {}
        virtual void update() {}

    private:
        void internalStart();
        void internalUpdate();

};