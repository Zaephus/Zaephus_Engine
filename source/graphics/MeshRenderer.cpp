
#include "MeshRenderer.h"

#include <string>

#include <ZMath.h>
#include <glad/gl.h>

#include "Action.h"
#include "Camera.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "Shader.h"
#include "TimeUtils.h"
#include "Transform.h"

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

Action<void(MeshRenderer*)> MeshRenderer::meshRendererCreatedCall = Action<void(MeshRenderer*)>();
Action<void(MeshRenderer*)> MeshRenderer::meshRendererDestroyedCall = Action<void(MeshRenderer*)>();

MeshRenderer::MeshRenderer() : MeshRenderer(nullptr, nullptr) {}
MeshRenderer::MeshRenderer(Mesh* _mesh) : MeshRenderer(_mesh, nullptr) {}
MeshRenderer::MeshRenderer(const Model* _model) : MeshRenderer(_model->mesh, _model->shader) {}
MeshRenderer::MeshRenderer(Mesh* _mesh, Shader* _shader) {
    setMesh(_mesh);
    setShader(_shader);
}

MeshRenderer::~MeshRenderer() {
    beingDestroyedFlag = true;

    mesh = nullptr;
    shader = nullptr;

    meshRendererDestroyedCall.invoke(this);
}

void MeshRenderer::initialize() {
    mesh->initialize();
    shader->initialize();

    meshRendererCreatedCall.invoke(this);
}

void MeshRenderer::render() const {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("MeshRenderer::Render", 0xbe33ff);
#endif

    mesh->bind();
    if(mesh->isDynamic) { mesh->updateVertexData(); }

    shader->bind();

    shader->setFloat("TIME", Time::currentTime());

    const Matrix4x4 modelMatrix = transform->objectMatrix();
    shader->setMatrix4x4("modelMatrix", modelMatrix);

    const Matrix4x4 normalMatrix = transform->directionMatrix();
    shader->setMatrix4x4("normalMatrix", normalMatrix);

    shader->setMatrix4x4("viewMatrix", Camera::activeCam->viewMatrix());

    shader->setMatrix4x4("projectionMatrix", Camera::activeCam->projectionMatrix);

    shader->applyUniforms();

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

bool MeshRenderer::isCurrentlyBeingDestroyed() {
    return beingDestroyedFlag;
}