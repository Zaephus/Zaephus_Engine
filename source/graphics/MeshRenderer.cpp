
#include "MeshRenderer.h"

#include <string>

#include <ZMath.h>
#include <glad/gl.h>

#include <Action.h>
#include <Camera.h>
#include <Vertex.h>
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"

Action<void(MeshRenderer*)> MeshRenderer::modelCreatedCall = Action<void(MeshRenderer*)>();
Action<void(MeshRenderer*)> MeshRenderer::modelDestroyedCall = Action<void(MeshRenderer*)>();

MeshRenderer::MeshRenderer(Mesh* _mesh) {
    setMesh(_mesh);
}

MeshRenderer::MeshRenderer(const Model _model) {
    setMesh(_model.mesh);
    setShader(_model.shader);
}

MeshRenderer::MeshRenderer(Mesh* _mesh, Shader* _shader) {
    setMesh(_mesh);
    setShader(_shader);
}

MeshRenderer::~MeshRenderer() {
    modelDestroyedCall.invoke(this);

    delete mesh;
    delete shader;
}

void MeshRenderer::start() {
    modelCreatedCall.invoke(this);
}

void MeshRenderer::render() const {
    if(mesh->isDynamic) { mesh->updateVertexData(); }

    if(Mesh::activeMesh != mesh) {
        mesh->bind();
        Mesh::activeMesh = mesh;
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