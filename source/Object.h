
#pragma once

#include <typeinfo>
#include <string>

class Object {
    public:
        std::string name;

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