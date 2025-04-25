
#include "MultiMeshRenderer.h"

#include <iostream>

#include <ZMath.h>

#include <Camera.h>
#include <Mesh.h>
#include <Model.h>
#include <Shader.h>
#include <Transform.h>

#ifdef ENABLE_PROFILING
    #include "tracy/Tracy.hpp"
#endif

MultiMeshRenderer::MultiMeshRenderer(Mesh* _mesh, const int _instanceCount) {
    mesh = _mesh;
    instanceCount = _instanceCount;

    for(size_t i = 0; i < _instanceCount; i++) {
        instanceTransforms.push_back(new Transform());
    }
}

MultiMeshRenderer::MultiMeshRenderer(const Model _model, const int _instanceCount) {
    mesh = _model.mesh;
    shader = _model.shader;
    instanceCount = _instanceCount;

    for(size_t i = 0; i < _instanceCount; i++) {
        instanceTransforms.push_back(new Transform());
    }
}

MultiMeshRenderer::MultiMeshRenderer(Mesh* _mesh, Shader* _shader, const int _instanceCount) {
    mesh = _mesh;
    shader = _shader;
    instanceCount = _instanceCount;

    for(size_t i = 0; i < _instanceCount; i++) {
        instanceTransforms.push_back(new Transform());
    }
}

MultiMeshRenderer::~MultiMeshRenderer() {
    delete mesh;
    delete shader;
}

void MultiMeshRenderer::start() {
    initializeInstanceBuffer();
}

void MultiMeshRenderer::render() {
#ifdef ENABLE_PROFILING
    ZoneScopedN("multi-mesh render");
#endif

    updateInstanceBuffer();

    if(Mesh::activeMesh != mesh) {
        mesh->bind();
        Mesh::activeMesh = mesh;
    }

    const Matrix4x4 modelMatrix = transform->objectMatrix();
    shader->setMatrix4x4("modelMatrix", modelMatrix);

    const Matrix4x4 normalMatrix = modelMatrix.inverse().transposed();
    shader->setMatrix4x4("normalMatrix", normalMatrix);

    // if(Camera::activeCam->transform->hasChanged) {
        shader->setMatrix4x4("viewMatrix", Camera::activeCam->viewMatrix());
    // }
    // if(Camera::activeCam->projectionChanged) {
        shader->setMatrix4x4("projectionMatrix", Camera::activeCam->projectionMatrix);
    // }

    glDrawElementsInstanced(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr, instanceCount);
}

void MultiMeshRenderer::setMesh(Mesh* _mesh) { mesh = _mesh; }
Mesh* MultiMeshRenderer::getMesh() const { return mesh; }

void MultiMeshRenderer::setShader(Shader* _shader) { shader = _shader; }
Shader* MultiMeshRenderer::getShader() const { return shader; }

void MultiMeshRenderer::setInstancePosition(const unsigned int _id, const Vector3& _pos) {
    if(_id >= instanceTransforms.size()) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    instanceTransforms[_id]->position = _pos;
}

void MultiMeshRenderer::setInstanceRotation(const unsigned int _id, const Quaternion& _rot) {
    if(_id >= instanceTransforms.size()) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    instanceTransforms[_id]->rotation = _rot;
}

void MultiMeshRenderer::setInstanceRotation(const unsigned int _id, const Vector3& _eulerAngles) {
    if(_id >= instanceTransforms.size()) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    instanceTransforms[_id]->rotation = Quaternion::fromEuler(_eulerAngles);
}

void MultiMeshRenderer::setInstanceScale(const unsigned int _id, const Vector3& _scale) {
    if(_id >= instanceTransforms.size()) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    instanceTransforms[_id]->scale = _scale;
}

void MultiMeshRenderer::rotateInstance(unsigned int _id, const Vector3& _eulerAngles) {
    if(_id >= instanceTransforms.size()) {
        std::cerr << "Index out of bounds." << std::endl;
    }

    instanceTransforms[_id]->rotate(_eulerAngles);
}

void MultiMeshRenderer::initializeInstanceBuffer() {
    glGenBuffers(1, &instanceBuffer);
    updateInstanceBuffer();

    if(Mesh::activeMesh != mesh) {
        mesh->bind();
        Mesh::activeMesh = mesh;
    }

    const int vec4Size = sizeof(Vector4);

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

void MultiMeshRenderer::updateInstanceBuffer() {
#ifdef ENABLE_PROFILING
    ZoneScopedN("update buffer");
#endif

    std::vector<Matrix4x4> matrices;
    for(size_t i = 0; i < instanceTransforms.size(); i++) {
        matrices.push_back(instanceTransforms[i]->objectMatrix().transposed());
    }

#ifdef ENABLE_PROFILING
    ZoneNamedN(BindZone, "bind buffer", true);
#endif

    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(Matrix4x4), matrices.data(), mesh->drawType);
}