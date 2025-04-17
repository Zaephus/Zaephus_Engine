
#include "Mesh.h"

#include <iostream>

#include <glad/gl.h>

#include <assimp/mesh.h>

#include "Shader.h"
#include "Vertex.h"

Mesh::Mesh() : Mesh({}, {}, {}, {}, {}) {}

Mesh::Mesh(const std::vector<Vector3>& _positions,
           const std::vector<Color>& _colors,
           const std::vector<Vector2>& _uvs,
           const std::vector<Vector3>& _normals,
           const std::vector<unsigned int>& _indices) {

    positions = _positions;
    colors = _colors;
    uvs = _uvs;
    normals = _normals;

    indices = _indices;
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
}

void Mesh::initialize() {
    if(isDynamic) { drawType = GL_DYNAMIC_DRAW; }
    else { drawType = GL_STATIC_DRAW; }

    initializeArrayObject();
    initializeVertexBuffer();
    initializeElementBuffer();
}

void Mesh::bind() const {
    glBindVertexArray(vertexArrayObject);
}

std::vector<Vertex> Mesh::combineData() const {
    std::vector<Vertex> vertices;

    for(size_t i = 0; i < positions.size(); i++) {
        Vertex v {
            .position = positions[i],

            .color  = i < colors.size()  ? colors[i]  : Color::white(),
            .uv     = i < uvs.size()     ? uvs[i]     : Vector2::one(),
            .normal = i < normals.size() ? normals[i] : Vector3::one()
        };
        vertices.push_back(v);
    }

    return vertices;
}

void Mesh::initializeArrayObject() {
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
}

void Mesh::initializeVertexBuffer() {
    glGenBuffers(1, &vertexBufferObject);
    updateVertexData();
}

void Mesh::initializeElementBuffer() {
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

    const int indicesSize = indices.size() * sizeof(unsigned int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), drawType);
}

void Mesh::updateVertexData() const {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    const std::vector<Vertex> vertices = combineData();

    const int verticesSize = vertices.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices.data(), drawType);
}

bool operator==(const Mesh& _lhs, const Mesh& _rhs) {
    if(_lhs.positions.size() != _rhs.positions.size()) { return false; }
    for(int i = 0; i < _lhs.positions.size(); i++) {
        if(_lhs.positions[i] != _rhs.positions[i]) { return false; }
    }

    if(_lhs.colors.size() != _rhs.colors.size()) { return false; }
    for(int i = 0; i < _lhs.colors.size(); i++) {
        if(_lhs.colors[i] != _rhs.colors[i]) { return false; }
    }

    if(_lhs.uvs.size() != _rhs.uvs.size()) { return false; }
    for(int i = 0; i < _lhs.uvs.size(); i++) {
        if(_lhs.uvs[i] != _rhs.uvs[i]) { return false; }
    }

    if(_lhs.normals.size() != _rhs.normals.size()) { return false; }
    for(int i = 0; i < _lhs.normals.size(); i++) {
        if(_lhs.normals[i] != _rhs.normals[i]) { return false; }
    }

    if(_lhs.indices.size() != _rhs.indices.size()) { return false; }
    for(int i = 0; i < _lhs.indices.size(); i++) {
        if(_lhs.indices[i] != _rhs.indices[i]) { return false; }
    }

    return true;
}

bool operator!=(const Mesh& _lhs, const Mesh& _rhs) {
    return !(_lhs == _rhs);
}

bool operator==(const Mesh& _lhs, const aiMesh& _rhs) {
    if(_lhs.positions.size() != _rhs.mNumVertices) { return false; }

    for(unsigned int i = 0; i < _rhs.mNumVertices; i++) {
        if(_lhs.positions[i].x != _rhs.mVertices[i].x) { return false; }
        if(_lhs.positions[i].y != _rhs.mVertices[i].y) { return false; }
        if(_lhs.positions[i].z != _rhs.mVertices[i].z) { return false; }

        if(_rhs.HasVertexColors(0)) {
            if(_lhs.colors[i].r != _rhs.mColors[0][i].r) { return false; }
            if(_lhs.colors[i].g != _rhs.mColors[0][i].g) { return false; }
            if(_lhs.colors[i].b != _rhs.mColors[0][i].b) { return false; }
            if(_lhs.colors[i].a != _rhs.mColors[0][i].a) { return false; }
        }

        if(_rhs.HasTextureCoords(0)) {
            if(_lhs.uvs[i].x != _rhs.mTextureCoords[0][i].x) { return false; }
            if(_lhs.uvs[i].y != _rhs.mTextureCoords[0][i].y) { return false; }
        }

        if(_rhs.HasNormals()) {
            if(_lhs.normals[i].x != _rhs.mNormals[i].x) { return false; }
            if(_lhs.normals[i].y != _rhs.mNormals[i].y) { return false; }
            if(_lhs.normals[i].z != _rhs.mNormals[i].z) { return false; }
        }
    }

    return true;
}

bool operator!=(const Mesh& _lhs, const aiMesh& _rhs) {
    return !(_lhs == _rhs);
}