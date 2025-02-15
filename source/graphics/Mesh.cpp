
#include "Mesh.h"

#include <iostream>

#include <glad/gl.h>

#include "Camera.h"
#include "Shader.h"
#include "Vertex.h"

Mesh::Mesh() : Mesh(nullptr, {}, {}, {}, {}, {}) {}

Mesh::Mesh(Shader* _shader,
           const std::vector<Vector3>& _positions,
           const std::vector<Color>& _colors,
           const std::vector<Vector2>& _uvs,
           const std::vector<Vector3>& _normals,
           const std::vector<unsigned int>& _indices) {

    shader = _shader;

    positions = _positions;
    colors = _colors;
    uvs = _uvs;
    normals = _normals;

    indices = _indices;
}

Mesh::~Mesh() {
    delete shader;

    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
}

void Mesh::initialize() {
    processData();

    initializeArrayObject();
    initializeVertexBuffer();
    initializeElementBuffer();

    setVertexAttributes();
}

void Mesh::render(const Matrix4x4& _model) const {
    shader->use();

    shader->setMatrix4x4("modelMatrix", _model);

    Matrix4x4 normalMatrix = _model.inverse().transposed();
    shader->setMatrix4x4("normalMatrix", normalMatrix);

    shader->setMatrix4x4("viewMatrix", Camera::activeCam->viewMatrix());

    shader->setMatrix4x4("projectionMatrix", Camera::activeCam->projectionMatrix);

    glBindVertexArray(vertexArrayObject);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::processData() {
    vertices.resize(positions.size());

    for(size_t i = 0; i < positions.size(); i++) {
        vertices[i].position = positions[i];

        vertices[i].color  = i < colors.size()  ? colors[i]  : Color::white();
        vertices[i].uv     = i < uvs.size()     ? uvs[i]     : Vector2::one();
        vertices[i].normal = i < normals.size() ? normals[i] : Vector3::one();
    }
}

void Mesh::initializeArrayObject() {
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
}

void Mesh::initializeVertexBuffer() {
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    int verticesSize = vertices.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices.data(), GL_STATIC_DRAW);
}

void Mesh::initializeElementBuffer() {
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

    int indicesSize = indices.size() * sizeof(unsigned int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_STATIC_DRAW);
}

void Mesh::setVertexAttributes() {
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Vertex Colors
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // Vertex UVs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);

    // Vertex Normals
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);
}