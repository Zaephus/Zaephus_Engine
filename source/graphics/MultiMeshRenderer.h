
#pragma once

#include <atomic>

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

        MultiMeshRenderer();
        MultiMeshRenderer(Mesh* _mesh, unsigned int _instanceCount);
        MultiMeshRenderer(Model _model, unsigned int _instanceCount);
        MultiMeshRenderer(Mesh* _mesh, Shader* _shader, unsigned int _instanceCount);

        void render();

        void setMesh(Mesh* _mesh);
        [[nodiscard]] Mesh* getMesh() const;

        void setShader(Shader* _shader);
        [[nodiscard]] Shader* getShader() const;

        [[nodiscard]] Vector3 getInstancePosition(unsigned int _id) const;
        [[nodiscard]] Vector3 getInstanceScale(unsigned int _id) const;
        [[nodiscard]] Quaternion getInstanceRotation(unsigned int _id) const;
        [[nodiscard]] Matrix4x4 getInstanceRotationMatrix(unsigned int _id) const;
        [[nodiscard]] Matrix4x4 getInstanceMatrix(unsigned int _id) const;

        void setInstancePosition(unsigned int _id, float _x, float _y, float _z);
        void setInstancePosition(unsigned int _id, const Vector3& _pos);

        void setInstanceScale(unsigned int _id, float _x, float _y, float _z);
        void setInstanceScale(unsigned int _id, const Vector3& _scale);

        void setInstanceRotation(unsigned int _id, float _xDeg, float _yDeg, float _zDeg);
        void setInstanceRotation(unsigned int _id, const Vector3& _eulerAngles);
        void setInstanceRotation(unsigned int _id, const Quaternion& _rot);

        void setInstanceMatrix(unsigned int _id, const Matrix4x4& _mat);

        void translateInstance(unsigned int _id, float _x, float _y, float _z);
        void translateInstance(unsigned int _id, const Vector3& _translate);

        void scaleInstance(unsigned int _id, float _x, float _y, float _z);
        void scaleInstance(unsigned int _id, const Vector3& _scale);

        void rotateInstance(unsigned int _id, float _xDeg, float _yDeg, float _zDeg);
        void rotateInstance(unsigned int _id, const Vector3& _eulerAngles);
        void rotateInstance(unsigned int _id, const Quaternion& _rot);

    protected:
        void start() override;
        void initialize() override;
        void destroy() override;

    private:
        Mesh* mesh = nullptr;
        Shader* shader = nullptr;

        unsigned int instanceCount;

        unsigned int instanceBuffer = 0;

        std::atomic<bool> instancesNotAccessibleFlag = false;

        Matrix4x4* currentMatrixBuffer = nullptr;
        Matrix4x4* nextMatrixBuffer = nullptr;

        void initializeInstanceBuffer();

        void swapBuffers();
        void copyBuffer(const Matrix4x4* _source, Matrix4x4* _dest);

        void updateInstanceBuffer() const;
};