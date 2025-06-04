
#pragma once

#include "Component.h"
#include "RenderObject.h"

template <typename T>
class Action;

class Mesh;
class Shader;

struct Model;

class MeshRenderer : public Component, RenderObject {
    public:
        static Action<void(MeshRenderer*)> meshRendererCreatedCall;
        static Action<void(MeshRenderer*)> meshRendererDestroyedCall;

        MeshRenderer();
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

        void initialize() override;
        void destroy() override;
};