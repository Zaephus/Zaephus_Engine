
#include "MeshRenderer.h"

#include <string>

#include <ZMath.h>

#include <Action.h>
#include <Camera.h>
#include <Vertex.h>
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"

Action<void(MeshRenderer*)> MeshRenderer::modelCreatedCall = Action<void(MeshRenderer*)>();
Action<void(MeshRenderer*)> MeshRenderer::modelDestroyedCall = Action<void(MeshRenderer*)>();

Mesh* MeshRenderer::activeMesh = nullptr;

MeshRenderer::MeshRenderer(Mesh* _mesh) {
    setMesh(_mesh);
}

MeshRenderer::MeshRenderer(Mesh* _mesh, Shader* _shader) {
    setMesh(_mesh);
    setShader(_shader);
}

MeshRenderer::~MeshRenderer() {
    modelDestroyedCall.invoke(this);

    delete mesh;
}

void MeshRenderer::start() {
    setVertexAttributes();

    modelCreatedCall.invoke(this);
}

void MeshRenderer::render() const {
    if(mesh->isDynamic) { mesh->updateVertexData(); }

    if(activeMesh != mesh) {
        mesh->bind();
        activeMesh = mesh;
    }

    const Matrix4x4 modelMatrix = transform->objectMatrix();
    shader->setMatrix4x4("modelMatrix", modelMatrix);

    const Matrix4x4 normalMatrix = modelMatrix.inverse().transposed();
    shader->setMatrix4x4("normalMatrix", normalMatrix);

    if(Camera::activeCam->transform->hasChanged) {
        shader->setMatrix4x4("viewMatrix", Camera::activeCam->viewMatrix());
    }
    if(Camera::activeCam->projectionChanged) {
        shader->setMatrix4x4("projectionMatrix", Camera::activeCam->projectionMatrix);
    }

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
}

void MeshRenderer::setMesh(Mesh* _mesh) {
    mesh = _mesh;
}
Mesh* MeshRenderer::getMesh() const { return mesh; }

void MeshRenderer::setShader(Shader* _shader) {
    shader = _shader;
}
Shader* MeshRenderer::getShader() const { return shader; }

void MeshRenderer::setVertexAttributes() {
    // Vertex Positions
    // ReSharper disable once CppZeroValuedExpressionUsedAsNullPointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);

    // Vertex Colors
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);

    // Vertex UVs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);

    // Vertex Normals
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(3);
}