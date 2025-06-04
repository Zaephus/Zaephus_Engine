
#pragma once

#include <vector>

#include "MeshRenderer.h"

class Bounds;
struct Vector3;

class BoundsRenderer : public MeshRenderer {
    public:
        explicit BoundsRenderer(Bounds* _bounds);

        void update() override;

    private:
        Bounds* bounds;

        [[nodiscard]] std::vector<Vector3> calculateVertices() const;
        [[nodiscard]] std::vector<unsigned int> calculateIndices() const;
};