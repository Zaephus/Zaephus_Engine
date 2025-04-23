
#include "BoundsRenderer.h"

#include <ZMath.h>

#include "Bounds.h"
#include "Color.h"
#include "Mesh.h"
#include "Shader.h"

BoundsRenderer::BoundsRenderer(Bounds* _bounds) {
    bounds = _bounds;

    Mesh* boundsMesh = new Mesh();

    boundsMesh->isDynamic = true;

    const std::vector<Vector3> newVertices {
        bounds->getBottomLeftFront(),
        bounds->getBottomRightFront(),
        bounds->getTopLeftFront(),
        bounds->getTopRightFront(),
        bounds->getBottomLeftBack(),
        bounds->getBottomRightBack(),
        bounds->getTopLeftBack(),
        bounds->getTopRightBack()
    };
    boundsMesh->positions = newVertices;

    const std::vector<unsigned int> newIndices {
        //Top
        2, 6, 7,
        2, 3, 7,

        //Bottom
        0, 4, 5,
        0, 1, 5,

        //Left
        0, 2, 6,
        0, 4, 6,

        //Right
        1, 3, 7,
        1, 5, 7,

        //Front
        0, 2, 3,
        0, 1, 3,

        //Back
        4, 6, 7,
        4, 5, 7
    };
    boundsMesh->indices = newIndices;

    setMesh(boundsMesh);

    setShader(Shader::unlitShader(Color::yellow()));
    shader->drawAsWireframe = true;
}

void BoundsRenderer::update() {
    const std::vector<Vector3> newVertices {
        bounds->getBottomLeftFront(),
        bounds->getBottomRightFront(),
        bounds->getTopLeftFront(),
        bounds->getTopRightFront(),
        bounds->getBottomLeftBack(),
        bounds->getBottomRightBack(),
        bounds->getTopLeftBack(),
        bounds->getTopRightBack()
    };
    mesh->positions = newVertices;
}