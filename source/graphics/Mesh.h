
#pragma once

#include <vector>

struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix4x4;

struct Vertex;
class Color;

class Shader;

class Mesh {

    public:
        Shader* shader;

        std::vector<Vector3> positions;
        std::vector<Color> colors;
        std::vector<Vector2> uvs;
        std::vector<Vector3> normals;

        std::vector<unsigned int> indices;

        bool isDynamic = false;

        Mesh();
        Mesh(Shader* _shader,
             const std::vector<Vector3>& _positions,
             const std::vector<Color>& _colors,
             const std::vector<Vector2>& _uvs,
             const std::vector<Vector3>& _normals,
             const std::vector<unsigned int>& _indices);

        ~Mesh();

        void initialize();
        void render(const Matrix4x4& _model);

    private:
        unsigned int vertexArrayObject = 0;

        unsigned int vertexBufferObject = 0;
        unsigned int elementBufferObject = 0;

        int drawType = 0;

        std::vector<Vertex> vertices;

        void processData();

        void initializeArrayObject();
        void initializeVertexBuffer();
        void initializeElementBuffer();

        void setVertexAttributes() const;

};