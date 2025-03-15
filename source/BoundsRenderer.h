
#pragma once

#include "Bounds.h"
#include "MeshRenderer.h"

class BoundsRenderer : public MeshRenderer {
    public:
        explicit BoundsRenderer(Bounds* _bounds);

        void update() override;

    private:
        Mesh* mesh;
        Bounds* bounds;
};