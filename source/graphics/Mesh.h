
#pragma once

#include <vector>

struct aiMesh;

struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix4x4;

struct Vertex;
struct Color;

class Shader;

class Mesh {
    public:
        std::vector<Vector3> positions;
        std::vector<Color> colors;
        std::vector<Vector2> uvs;
        std::vector<Vector3> normals;

        std::vector<unsigned int> indices;

        bool isDynamic = false;
        int drawType = 0;

        static Mesh* activeMesh;

        Mesh();
        Mesh(const std::vector<Vector3>& _positions,
             const std::vector<Color>& _colors,
             const std::vector<Vector2>& _uvs,
             const std::vector<Vector3>& _normals,
             const std::vector<unsigned int>& _indices);

        void initialize();
        void destroy();

        void bindToModel();

        void bind() const;

        void updateVertexData() const;

        friend bool operator==(const Mesh& _lhs, const Mesh& _rhs);
        friend bool operator!=(const Mesh& _lhs, const Mesh& _rhs);

        friend bool operator==(const Mesh& _lhs, const aiMesh& _rhs);
        friend bool operator!=(const Mesh& _lhs, const aiMesh& _rhs);

    private:
        int boundAmount = 0;

        unsigned int vertexArrayObject = 0;
        unsigned int vertexBufferObject = 0;
        unsigned int elementBufferObject = 0;

        ~Mesh() = default;

        [[nodiscard]] std::vector<Vertex> combineData() const;

        void initializeArrayObject();
        void initializeVertexBuffer();
        void initializeElementBuffer();

        static void setVertexAttributes();
};