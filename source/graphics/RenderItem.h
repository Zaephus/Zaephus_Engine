
#pragma once

class RenderItem {
    public:
        RenderItem();
        virtual ~RenderItem();

    protected:
        virtual void initialize() {}

    private:
        void internalInitialize();

};