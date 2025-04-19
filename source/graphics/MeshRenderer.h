
#pragma once

#include "Component.h"

template <typename T>
class Action;

class Color;
class Light;
class Mesh;
class Shader;
class Texture2D;

struct Model;
struct Matrix4x4;
struct Vector3;
struct Vertex;

class MeshRenderer : public Component {
    public:
        static Action<void(MeshRenderer*)> modelCreatedCall;
        static Action<void(MeshRenderer*)> modelDestroyedCall;

        MeshRenderer() = default;
        explicit MeshRenderer(Mesh* _mesh);
        explicit MeshRenderer(Model _model);
        MeshRenderer(Mesh* _mesh, Shader* _shader);
        ~MeshRenderer() override;

        void render() const;

        void setMesh(Mesh* _mesh);
        [[nodiscard]] Mesh* getMesh() const;

        void setShader(Shader* _shader);
        [[nodiscard]] Shader* getShader() const;

    protected:
        Mesh* mesh = nullptr;
        Shader* shader = nullptr;

        void start() override;
};