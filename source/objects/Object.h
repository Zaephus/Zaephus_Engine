
#pragma once

#include <typeinfo>
#include <string>

class Object {
    public:
        std::string name;

        Object();
        virtual ~Object();

        /**
         * Gives the type for the object.
         * @return The type of the object.
         */
        virtual const std::type_info& getType();

    protected:
        /**
         * Empty function that can be extended to handle setting up or initializing the object. This function is called at the start of the first frame after the object's creation.
         * @note This function is called automatically by the engine, it should never be necessary to call it manually.
         */
        virtual void start() {}

        /**
         * Empty function that can be extended to handle behavior that needs to happen every frame for this object. This function is called every frame before rendering.
         * @note This function is called automatically by the engine, it should never be necessary to call it manually.
         */
        virtual void update() {}

    private:
        void internalStart();
        void internalUpdate();

};