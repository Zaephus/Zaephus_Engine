
#pragma once

#include "MeshRenderer.h"

class Bounds;

class BoundsRenderer : public MeshRenderer {
    public:
        explicit BoundsRenderer(Bounds* _bounds);

        void update() override;

    private:
        Bounds* bounds;
};