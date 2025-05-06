
#pragma once

#include <vector>

#include "Component.h"

class Transform;
class Mesh;
class Shader;

struct Model;
struct Vector3;
struct Quaternion;
struct Matrix4x4;

class MultiMeshRenderer : public Component {
    public:
        MultiMeshRenderer(Mesh* _mesh, int _instanceCount);
        MultiMeshRenderer(Model _model, int _instanceCount);
        MultiMeshRenderer(Mesh* _mesh, Shader* _shader, int _instanceCount);
        ~MultiMeshRenderer() override;

        void render();

        void setMesh(Mesh* _mesh);
        [[nodiscard]] Mesh* getMesh() const;

        void setShader(Shader* _shader);
        [[nodiscard]] Shader* getShader() const;

        [[nodiscard]] Vector3 getInstancePosition(unsigned int _id) const;

        void setInstancePosition(unsigned int _id, const Vector3& _pos);
        void setInstanceRotation(unsigned int _id, const Vector3& _eulerAngles);
        void setInstanceRotation(unsigned int _id, const Quaternion& _rot);
        void setInstanceScale(unsigned int _id, const Vector3& _scale);
        // void setInstanceMatrix(unsigned int _id, const Matrix4x4& _mat);

        void rotateInstance(unsigned int _id, const Vector3& _eulerAngles);

    protected:
        void start() override;

    private:
        Mesh* mesh = nullptr;
        Shader* shader = nullptr;

        int instanceCount;

        unsigned int instanceBuffer = 0;

        std::vector<Transform*> instanceTransforms;
        std::vector<Matrix4x4> matrices;

        void initializeInstanceBuffer();
        void updateInstanceBuffer();
};