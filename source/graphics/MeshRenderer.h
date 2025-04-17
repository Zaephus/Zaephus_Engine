
#pragma once

#include "Component.h"

template <typename T>
class Action;

class Color;
class Light;
class Mesh;
class Shader;
class Texture2D;

struct Matrix4x4;
struct Vector3;
struct Vertex;

class MeshRenderer : public Component {
    public:
        static Action<void(MeshRenderer*)> modelCreatedCall;
        static Action<void(MeshRenderer*)> modelDestroyedCall;

        MeshRenderer() = default;
        explicit MeshRenderer(Mesh* _mesh);
        MeshRenderer(Mesh* _mesh, Shader* _shader);
        ~MeshRenderer() override;

        void render() const;

        void setMesh(Mesh* _mesh);
        Mesh* getMesh() const;

        void setShader(Shader* _shader);
        Shader* getShader() const;

    protected:
        Mesh* mesh = nullptr;
        Shader* shader = nullptr;

        void start() override;

    private:
        static Mesh* activeMesh;

        static void setVertexAttributes();
};