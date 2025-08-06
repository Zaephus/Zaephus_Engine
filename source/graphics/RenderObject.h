
#pragma once

class RenderObject {
    public:
        RenderObject();
        virtual ~RenderObject();

    protected:
        virtual void initialize() {}

    private:
        void internalInitialize();

};