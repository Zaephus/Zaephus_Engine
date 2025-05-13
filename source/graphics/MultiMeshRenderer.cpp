
#include "MultiMeshRenderer.h"

#include <iostream>

#include <ZMath.h>
#include <glad/gl.h>

#include "Action.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"

#ifdef ENABLE_PROFILING
    #include <tracy/Tracy.hpp>
#endif

Action<void(MultiMeshRenderer*)> MultiMeshRenderer::multiMeshRendererCreatedCall = Action<void(MultiMeshRenderer*)>();
Action<void(MultiMeshRenderer*)> MultiMeshRenderer::multiMeshRendererDestroyedCall = Action<void(MultiMeshRenderer*)>();

MultiMeshRenderer::MultiMeshRenderer(Mesh* _mesh, const int _instanceCount)
    : MultiMeshRenderer(_mesh, nullptr, _instanceCount) {}

MultiMeshRenderer::MultiMeshRenderer(const Model _model, const int _instanceCount)
    : MultiMeshRenderer(_model.mesh, _model.shader, _instanceCount) {}

MultiMeshRenderer::MultiMeshRenderer(Mesh* _mesh, Shader* _shader, const int _instanceCount) {
    mesh = _mesh;
    shader = _shader;
    instanceCount = _instanceCount;

    for(size_t i = 0; i < _instanceCount; i++) {
        matrices.push_back(Matrix4x4::identity());
    }
}

MultiMeshRenderer::~MultiMeshRenderer() {
    multiMeshRendererDestroyedCall.invoke(this);

    delete mesh;
    delete shader;
}

void MultiMeshRenderer::start() {
    multiMeshRendererCreatedCall.invoke(this);
}

void MultiMeshRenderer::initialize() {
    initializeInstanceBuffer();
}

void MultiMeshRenderer::render() const {
#ifdef ENABLE_PROFILING
    ZoneScopedN("multi-mesh render");
#endif

    if(!shader->isInitialized()) { return; }

    shader->bind();

    updateInstanceBuffer();

    if(Mesh::activeMesh != mesh) {
        mesh->bind();
        Mesh::activeMesh = mesh;
    }

    const Matrix4x4 modelMatrix = transform->objectMatrix();
    shader->setMatrix4x4("modelMatrix", modelMatrix);

    const Matrix4x4 normalMatrix = modelMatrix.inverse().transposed();
    shader->setMatrix4x4("normalMatrix", normalMatrix);

    shader->setMatrix4x4("viewMatrix", Camera::activeCam->viewMatrix());

    shader->setMatrix4x4("projectionMatrix", Camera::activeCam->projectionMatrix);

    shader->applyUniforms();

    glDrawElementsInstanced(GL_TRIANGLES, static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, nullptr, instanceCount);
}

void MultiMeshRenderer::setMesh(Mesh* _mesh) { mesh = _mesh; }
Mesh* MultiMeshRenderer::getMesh() const { return mesh; }

void MultiMeshRenderer::setShader(Shader* _shader) { shader = _shader; }
Shader* MultiMeshRenderer::getShader() const { return shader; }

Vector3 MultiMeshRenderer::getInstancePosition(const unsigned int _id) const {
    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
    }
    return { matrices[_id].m03, matrices[_id].m13, matrices[_id].m23 };
}

void MultiMeshRenderer::setInstancePosition(const unsigned int _id, const Vector3& _pos) {
    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    matrices[_id].m03 = _pos.x;
    matrices[_id].m13 = _pos.y;
    matrices[_id].m23 = _pos.z;
}

void MultiMeshRenderer::setInstanceRotation(const unsigned int _id, const Vector3& _eulerAngles) {
    setInstanceRotation(_id, Quaternion::fromEuler(_eulerAngles));
}
void MultiMeshRenderer::setInstanceRotation(const unsigned int _id, const Quaternion& _rot) {
    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    const Matrix4x4 rotMatrix = Matrix4x4::rotateMatrix(_rot);

    matrices[_id].m00 = rotMatrix.m00; matrices[_id].m01 = rotMatrix.m01; matrices[_id].m02 = rotMatrix.m02;
    matrices[_id].m10 = rotMatrix.m10; matrices[_id].m11 = rotMatrix.m11; matrices[_id].m12 = rotMatrix.m12;
    matrices[_id].m20 = rotMatrix.m20; matrices[_id].m21 = rotMatrix.m21; matrices[_id].m22 = rotMatrix.m22;
}

void MultiMeshRenderer::rotateInstance(const unsigned int _id, const Vector3& _eulerAngles) {
    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    const Matrix4x4 rotMatrix = Matrix4x4::rotateMatrix(Quaternion::fromEuler(_eulerAngles));
    matrices[_id] *= rotMatrix;
}

void MultiMeshRenderer::initializeInstanceBuffer() {
    glGenBuffers(1, &instanceBuffer);
    updateInstanceBuffer();

    if(Mesh::activeMesh != mesh) {
        mesh->bind();
        Mesh::activeMesh = mesh;
    }

    constexpr int vec4Size = sizeof(Vector4);

    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);

    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(6, 1);

    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(3 * vec4Size));
    glEnableVertexAttribArray(7);
    glVertexAttribDivisor(7, 1);
}

void MultiMeshRenderer::updateInstanceBuffer() const {
#ifdef ENABLE_PROFILING
    ZoneScopedN("update buffer");
#endif

    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(instanceCount * sizeof(Matrix4x4)), matrices.data(), mesh->drawType);
}