
#include "MultiMeshRenderer.h"

#include <iostream>

#include <ZMath.h>
#include <glad/gl.h>

#include "Action.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Scene.h"
#include "Shader.h"
#include "Transform.h"

#ifdef ENABLE_PROFILING
    #include <tracy/Tracy.hpp>
#endif

Action<void(MultiMeshRenderer*)> MultiMeshRenderer::multiMeshRendererCreatedCall = Action<void(MultiMeshRenderer*)>();
Action<void(MultiMeshRenderer*)> MultiMeshRenderer::multiMeshRendererDestroyedCall = Action<void(MultiMeshRenderer*)>();

MultiMeshRenderer::MultiMeshRenderer() {
    mesh = nullptr;
    shader = nullptr;

    instanceCount = 0;
}

MultiMeshRenderer::MultiMeshRenderer(Mesh* _mesh, const int _instanceCount)
    : MultiMeshRenderer(_mesh, nullptr, _instanceCount) {}

MultiMeshRenderer::MultiMeshRenderer(const Model _model, const int _instanceCount)
    : MultiMeshRenderer(_model.mesh, _model.shader, _instanceCount) {}

MultiMeshRenderer::MultiMeshRenderer(Mesh* _mesh, Shader* _shader, const int _instanceCount) {
    mesh = _mesh;
    shader = _shader;
    instanceCount = _instanceCount;

    prevMatrixBuffer = new Matrix4x4[_instanceCount];
    currentMatrixBuffer = new Matrix4x4[_instanceCount];
    nextMatrixBuffer = new Matrix4x4[_instanceCount];

    // matrices.resize(_instanceCount);
    // bufferedMatrices.resize(_instanceCount);
}

MultiMeshRenderer::~MultiMeshRenderer() {
    multiMeshRendererDestroyedCall.invoke(this);

    delete mesh;
    delete shader;
}

void MultiMeshRenderer::start() {
    multiMeshRendererCreatedCall.invoke(this);

     copyBuffers(nextMatrixBuffer, prevMatrixBuffer);
    copyBuffers(nextMatrixBuffer, currentMatrixBuffer);

    Scene::activeScene->notifyEndOfFrame.bind<MultiMeshRenderer, &MultiMeshRenderer::swapPrevNext>(this);
}

void MultiMeshRenderer::initialize() {
    mesh->initialize();
    shader->initialize();

    initializeInstanceBuffer();
}

void MultiMeshRenderer::render() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("MultiMeshRenderer::Render",0x33d6ff);
#endif

    mesh->bind();
    shader->bind();

    // copyInstanceBuffer();
    swapCurrentPrev();
    updateInstanceBuffer();

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

Vector3 MultiMeshRenderer::getInstancePosition(const unsigned int _id) {
    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
    }
    return { nextMatrixBuffer[_id].m03, nextMatrixBuffer[_id].m13, nextMatrixBuffer[_id].m23 };
}

void MultiMeshRenderer::setInstancePosition(const unsigned int _id, const Vector3& _pos) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    nextMatrixBuffer[_id].m03 = _pos.x;
    nextMatrixBuffer[_id].m13 = _pos.y;
    nextMatrixBuffer[_id].m23 = _pos.z;
}

void MultiMeshRenderer::setInstanceRotation(const unsigned int _id, const Vector3& _eulerAngles) {
    setInstanceRotation(_id, Quaternion::fromEuler(_eulerAngles));
}
void MultiMeshRenderer::setInstanceRotation(const unsigned int _id, const Quaternion& _rot) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    const Matrix4x4 rotMatrix = Matrix4x4::rotateMatrix(_rot);

    nextMatrixBuffer[_id].m00 = rotMatrix.m00; nextMatrixBuffer[_id].m01 = rotMatrix.m01; nextMatrixBuffer[_id].m02 = rotMatrix.m02;
    nextMatrixBuffer[_id].m10 = rotMatrix.m10; nextMatrixBuffer[_id].m11 = rotMatrix.m11; nextMatrixBuffer[_id].m12 = rotMatrix.m12;
    nextMatrixBuffer[_id].m20 = rotMatrix.m20; nextMatrixBuffer[_id].m21 = rotMatrix.m21; nextMatrixBuffer[_id].m22 = rotMatrix.m22;
}

void MultiMeshRenderer::setInstanceMatrix(const unsigned int _id, const Matrix4x4& _mat) {
    while(instancesNotAccessibleFlag == true) {}

    nextMatrixBuffer[_id] = _mat;
}

void MultiMeshRenderer::rotateInstance(const unsigned int _id, const Vector3& _eulerAngles) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    const Matrix4x4 rotMatrix = Matrix4x4::rotateMatrix(Quaternion::fromEuler(_eulerAngles));
    const Matrix4x4 result = currentMatrixBuffer[_id] * rotMatrix;
    nextMatrixBuffer[_id] = result;
}

void MultiMeshRenderer::initializeInstanceBuffer() {
    glGenBuffers(1, &instanceBuffer);
    // copyInstanceBuffer();
    updateInstanceBuffer();

    mesh->bind();

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

void MultiMeshRenderer::swapPrevNext() {
    std::swap(prevMatrixBuffer, nextMatrixBuffer);
}

void MultiMeshRenderer::swapCurrentPrev() {
    std::swap(currentMatrixBuffer, prevMatrixBuffer);
}

void MultiMeshRenderer::copyBuffers(const Matrix4x4* _from, Matrix4x4* _to) const {
    memcpy(_to, _from, sizeof(Matrix4x4) * instanceCount);
}

void MultiMeshRenderer::copyInstanceBuffer() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("MultiMeshRenderer::CopyInstanceBuffer", 0x33d6ff);
#endif

    instancesNotAccessibleFlag = true;

    // memcpy(bufferedMatrices, matrices, sizeof(Matrix4x4) * instanceCount);

    std::swap(prevMatrixBuffer, nextMatrixBuffer);
    std::swap(currentMatrixBuffer, prevMatrixBuffer);

    // std::swap(matrices, bufferedMatrices);
    
    // for(size_t i = 0; i < matrices.size(); i++) {
    //     // bufferedMatrices[i] = matrices[i];
    //     memcpy(&bufferedMatrices[i], &matrices[i], sizeof(Matrix4x4));
    // }

    instancesNotAccessibleFlag = false;
}

void MultiMeshRenderer::updateInstanceBuffer() const {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("MultiMeshRenderer::UpdateInstanceBuffer", 0x33d6ff);
#endif

    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(instanceCount * sizeof(Matrix4x4)), currentMatrixBuffer, mesh->drawType);
}