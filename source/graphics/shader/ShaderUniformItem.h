
#pragma once

struct Color;
struct Vector3;
struct Matrix4x4;

class ShaderUniformItem {
    public:
        const int* intValue = nullptr;
        const float* floatValue = nullptr;
        const Color* colorValue = nullptr;
        const Vector3* vector3Value = nullptr;
        const Matrix4x4* matrixValue = nullptr;

        void apply(int _id, const char*& _name) const;

    private:
        void setInt(int _id, const char*& _name) const;
        void setFloat(int _id, const char*& _name) const;
        void setColor(int _id, const char*& _name) const;
        void setVector3(int _id, const char*& _name) const;
        void setMatrix4x4(int _id, const char*& _name) const;
};