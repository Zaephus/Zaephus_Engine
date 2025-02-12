
#pragma once

#include "Bounds.h"
#include "MeshRenderer.h"

class BoundsRenderer : public MeshRenderer {
    public:
        explicit BoundsRenderer(Bounds* _bounds);

    private:
        Bounds* bounds;
};