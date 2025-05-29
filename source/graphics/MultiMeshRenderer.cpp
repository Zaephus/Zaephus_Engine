
#include "MultiMeshRenderer.h"

#include <iostream>

#include <ZMath.h>
#include <glad/gl.h>

#include "Action.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "TimeUtils.h"
#include "Transform.h"

#ifdef ENABLE_PROFILING
    #include <tracy/Tracy.hpp>
#endif

Action<void(MultiMeshRenderer*)> MultiMeshRenderer::multiMeshRendererCreatedCall = Action<void(MultiMeshRenderer*)>();

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

    nextMatrixBuffer = new Matrix4x4[_instanceCount];
    currentMatrixBuffer = nextMatrixBuffer;
}

void MultiMeshRenderer::start() {
    multiMeshRendererCreatedCall.invoke(this);

    currentMatrixBuffer = new Matrix4x4[instanceCount];
    swapBuffers();
}

void MultiMeshRenderer::initialize() {
    mesh->initialize();
    shader->initialize();

    initializeInstanceBuffer();
}

void MultiMeshRenderer::destroy() {
    mesh->destroy();
    shader->destroy();
}

void MultiMeshRenderer::render() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("MultiMeshRenderer::Render",0x33d6ff);
#endif

    mesh->bind();
    shader->bind();
    updateInstanceBuffer();

    shader->setFloat("TIME", Time::currentTime());

    const Matrix4x4 modelMatrix = transform->objectMatrix();
    shader->setMatrix4x4("modelMatrix", modelMatrix);

    const Matrix4x4 normalMatrix = modelMatrix.inverse().transposed();
    shader->setMatrix4x4("normalMatrix", normalMatrix);

    shader->setMatrix4x4("viewMatrix", Camera::activeCam->viewMatrix());

    shader->setMatrix4x4("projectionMatrix", Camera::activeCam->projectionMatrix);

    shader->applyUniforms();

    glDrawElementsInstanced(GL_TRIANGLES, static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, nullptr, instanceCount);

    swapBuffers();
}

void MultiMeshRenderer::setMesh(Mesh* _mesh) { mesh = _mesh; }
Mesh* MultiMeshRenderer::getMesh() const { return mesh; }

void MultiMeshRenderer::setShader(Shader* _shader) { shader = _shader; }
Shader* MultiMeshRenderer::getShader() const { return shader; }

Vector3 MultiMeshRenderer::getInstancePosition(const unsigned int _id) const {
    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return Vector3::zero();
    }
    return { nextMatrixBuffer[_id].m03, nextMatrixBuffer[_id].m13, nextMatrixBuffer[_id].m23 };
}

Vector3 MultiMeshRenderer::getInstanceScale(const unsigned int _id) const {
    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return Vector3::zero();
    }

    const Matrix4x4 m = nextMatrixBuffer[_id];

    float sx = Vector3(m.m00, m.m10, m.m20).magnitude();
    float sy = Vector3(m.m01, m.m11, m.m21).magnitude();
    float sz = Vector3(m.m02, m.m12, m.m22).magnitude();

    return { sx, sy, sz };
}

Quaternion MultiMeshRenderer::getInstanceRotation(const unsigned int _id) const {
    return Quaternion(getInstanceRotationMatrix(_id));
}
Matrix4x4 MultiMeshRenderer::getInstanceRotationMatrix(const unsigned int _id) const {
    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return Matrix4x4::identity();
    }

    const Vector3 s = getInstanceScale(_id);
    const Matrix4x4 m = nextMatrixBuffer[_id];

    return {
        m.m00/s.x, m.m01/s.y, m.m02/s.z, 0.0f,
        m.m10/s.x, m.m11/s.y, m.m12/s.z, 0.0f,
        m.m20/s.x, m.m21/s.y, m.m22/s.z, 0.0f,
        0.0f,      0.0f,      0.0f,      1.0f
    };
}

Matrix4x4 MultiMeshRenderer::getInstanceMatrix(const unsigned int _id) const {
    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return Matrix4x4::identity();
    }

    return nextMatrixBuffer[_id];
}

void MultiMeshRenderer::setInstancePosition(const unsigned int _id, const float _x, const float _y, const float _z) {
    setInstancePosition(_id, { _x, _y, _z });
}
void MultiMeshRenderer::setInstancePosition(const unsigned int _id, const Vector3& _pos) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return;
    }

    nextMatrixBuffer[_id].m03 = _pos.x;
    nextMatrixBuffer[_id].m13 = _pos.y;
    nextMatrixBuffer[_id].m23 = _pos.z;
}

void MultiMeshRenderer::setInstanceScale(const unsigned int _id, const float _x, const float _y, const float _z) {
    setInstanceScale(_id, { _x, _y, _z });
}
void MultiMeshRenderer::setInstanceScale(const unsigned int _id, const Vector3& _scale) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return;
    }

    const Matrix4x4 rotMatrix = getInstanceRotationMatrix(_id);
    const Matrix4x4 scaleMatrix = {
        _scale.x, 0.0f, 0.0f, currentMatrixBuffer[_id].m03,
        0.0f, _scale.y, 0.0f, currentMatrixBuffer[_id].m13,
        0.0f, 0.0f, _scale.z, currentMatrixBuffer[_id].m23,
        0.0f, 0.0f, 0.0f,     1.0f
    };

    nextMatrixBuffer[_id] = scaleMatrix * rotMatrix;
}

void MultiMeshRenderer::setInstanceRotation(const unsigned int _id, const float _xDeg, const float _yDeg, const float _zDeg) {
    setInstanceRotation(_id, Quaternion::fromEuler(_xDeg, _yDeg, _zDeg));
}
void MultiMeshRenderer::setInstanceRotation(const unsigned int _id, const Vector3& _eulerAngles) {
    setInstanceRotation(_id, Quaternion::fromEuler(_eulerAngles));
}
void MultiMeshRenderer::setInstanceRotation(const unsigned int _id, const Quaternion& _rot) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return;
    }

    const Vector3 scale = getInstanceScale(_id);

    const Matrix4x4 rotMatrix = Matrix4x4::rotateMatrix(_rot);

    nextMatrixBuffer[_id].m00 = rotMatrix.m00; nextMatrixBuffer[_id].m01 = rotMatrix.m01; nextMatrixBuffer[_id].m02 = rotMatrix.m02;
    nextMatrixBuffer[_id].m10 = rotMatrix.m10; nextMatrixBuffer[_id].m11 = rotMatrix.m11; nextMatrixBuffer[_id].m12 = rotMatrix.m12;
    nextMatrixBuffer[_id].m20 = rotMatrix.m20; nextMatrixBuffer[_id].m21 = rotMatrix.m21; nextMatrixBuffer[_id].m22 = rotMatrix.m22;

    nextMatrixBuffer[_id] *= Matrix4x4::scaleMatrix(scale);
}

void MultiMeshRenderer::setInstanceMatrix(const unsigned int _id, const Matrix4x4& _mat) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return;
    }

    nextMatrixBuffer[_id] = _mat;
}

void MultiMeshRenderer::translateInstance(const unsigned int _id, const float _x, const float _y, const float _z) {
    translateInstance(_id, { _x, _y, _z });
}
void MultiMeshRenderer::translateInstance(const unsigned int _id, const Vector3& _translate) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return;
    }

    const Matrix4x4 translateMatrix = Matrix4x4::translateMatrix(_translate);
    const Matrix4x4 result = currentMatrixBuffer[_id] * translateMatrix;
    nextMatrixBuffer[_id] = result;
}

void MultiMeshRenderer::scaleInstance(unsigned int _id, float _x, float _y, float _z) {
    scaleInstance(_id, { _x, _y, _z });
}
void MultiMeshRenderer::scaleInstance(const unsigned int _id, const Vector3& _scale) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return;
    }

    const Matrix4x4 scaleMatrix = Matrix4x4::scaleMatrix(_scale);
    const Matrix4x4 result = currentMatrixBuffer[_id] * scaleMatrix;
    nextMatrixBuffer[_id] = result;
}

void MultiMeshRenderer::rotateInstance(const unsigned int _id, const float _xDeg, const float _yDeg, const float _zDeg) {
    rotateInstance(_id, Quaternion::fromEuler(_xDeg, _yDeg, _zDeg));
}
void MultiMeshRenderer::rotateInstance(const unsigned int _id, const Vector3& _eulerAngles) {
    rotateInstance(_id, Quaternion::fromEuler(_eulerAngles));
}
void MultiMeshRenderer::rotateInstance(const unsigned int _id, const Quaternion& _rot) {
    while(instancesNotAccessibleFlag == true) {}

    if(_id >= instanceCount) {
        std::cerr << "Index out of bounds." << std::endl;
        return;
    }

    const Matrix4x4 rotMatrix = Matrix4x4::rotateMatrix(_rot);
    const Matrix4x4 result = currentMatrixBuffer[_id] * rotMatrix;
    nextMatrixBuffer[_id] = result;
}

void MultiMeshRenderer::initializeInstanceBuffer() {
    glGenBuffers(1, &instanceBuffer);
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

void MultiMeshRenderer::swapBuffers() {
    std::swap(currentMatrixBuffer, nextMatrixBuffer);
}

void MultiMeshRenderer::updateInstanceBuffer() const {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("MultiMeshRenderer::UpdateInstanceBuffer", 0x33d6ff);
#endif

    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(instanceCount * sizeof(Matrix4x4)), currentMatrixBuffer, mesh->drawType);
}