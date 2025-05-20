
#pragma once

#include <atomic>
#include <vector>

#include "Component.h"
#include "RenderItem.h"
#include "Matrices/Matrix4x4.h"

template <typename T>
class Action;

class Transform;
class Mesh;
class Shader;

struct Model;
struct Vector3;
struct Quaternion;
struct Matrix4x4;

class MultiMeshRenderer : public Component, RenderItem {
    public:
        static Action<void(MultiMeshRenderer*)> multiMeshRendererCreatedCall;
        static Action<void(MultiMeshRenderer*)> multiMeshRendererDestroyedCall;

        MultiMeshRenderer();
        MultiMeshRenderer(Mesh* _mesh, int _instanceCount);
        MultiMeshRenderer(Model _model, int _instanceCount);
        MultiMeshRenderer(Mesh* _mesh, Shader* _shader, int _instanceCount);
        ~MultiMeshRenderer() override;

        void render();

        void setMesh(Mesh* _mesh);
        [[nodiscard]] Mesh* getMesh() const;

        void setShader(Shader* _shader);
        [[nodiscard]] Shader* getShader() const;

        [[nodiscard]] Vector3 getInstancePosition(unsigned int _id);

        void setInstancePosition(unsigned int _id, const Vector3& _pos);
        void setInstanceRotation(unsigned int _id, const Vector3& _eulerAngles);
        void setInstanceRotation(unsigned int _id, const Quaternion& _rot);
        void setInstanceMatrix(unsigned int _id, const Matrix4x4& _mat);

        void rotateInstance(unsigned int _id, const Vector3& _eulerAngles);

    protected:
        void start() override;
        void initialize() override;

    private:
        Mesh* mesh = nullptr;
        Shader* shader = nullptr;

        int instanceCount;

        unsigned int instanceBuffer = 0;

        std::atomic<bool> instancesNotAccessibleFlag = false;

        // std::vector<Matrix4x4> matrices;
        // std::vector<Matrix4x4> bufferedMatrices;

        Matrix4x4* prevMatrixBuffer = nullptr;
        Matrix4x4* currentMatrixBuffer = nullptr;
        Matrix4x4* nextMatrixBuffer = nullptr;

        void initializeInstanceBuffer();

        void swapPrevNext();
        void swapCurrentPrev();

        void copyBuffers(const Matrix4x4* _from, Matrix4x4* _to) const;

        void copyInstanceBuffer();
        void updateInstanceBuffer() const;
};