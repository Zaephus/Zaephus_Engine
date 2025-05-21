
#pragma once

class RenderItem {
    public:
        RenderItem();
        virtual ~RenderItem();

    protected:
        virtual void initialize() {}
        virtual void destroy() {}

    private:
        void internalInitialize();
        void internalDestroy();

};