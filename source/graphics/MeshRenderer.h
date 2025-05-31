
#pragma once

#include "Component.h"
#include "RenderItem.h"

template <typename T>
class Action;

class Color;
class PointLight;
class Mesh;
class Shader;
class Texture2D;

struct Model;
struct Matrix4x4;
struct Vector3;
struct Vertex;

class MeshRenderer : public Component, RenderItem {
    public:
        static Action<void(MeshRenderer*)> meshRendererCreatedCall;

        MeshRenderer() = default;
        explicit MeshRenderer(Mesh* _mesh);
        explicit MeshRenderer(Model _model);
        MeshRenderer(Mesh* _mesh, Shader* _shader);

        void render() const;

        void setMesh(Mesh* _mesh);
        [[nodiscard]] Mesh* getMesh() const;

        void setShader(Shader* _shader);
        [[nodiscard]] Shader* getShader() const;

    protected:
        Mesh* mesh = nullptr;
        Shader* shader = nullptr;

        void start() override;
        void initialize() override;
        void destroy() override;
};