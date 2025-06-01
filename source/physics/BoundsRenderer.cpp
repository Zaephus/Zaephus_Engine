
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

    boundsMesh->positions = calculateVertices();
    boundsMesh->indices = calculateIndices();

    setMesh(boundsMesh);

    setShader(Shader::unlitShader(Color::yellow()));
    shader->drawAsWireframe = true;
}

void BoundsRenderer::update() {
    mesh->positions = calculateVertices();
}

std::vector<Vector3> BoundsRenderer::calculateVertices() const {
    return {
        bounds->getBottomLeftFront() * 1.00001,
        bounds->getBottomRightFront() * 1.00001,
        bounds->getTopLeftFront() * 1.00001,
        bounds->getTopRightFront() * 1.00001,
        bounds->getBottomLeftBack() * 1.00001,
        bounds->getBottomRightBack() * 1.00001,
        bounds->getTopLeftBack() * 1.00001,
        bounds->getTopRightBack() * 1.00001
    };
}

std::vector<unsigned int> BoundsRenderer::calculateIndices() const {
    return {
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
}