
#pragma once

#include <iostream>

#include <glad/gl.h>
#include <ZMath.h>

#include "Color.h"

class ShaderUniformItem {
    public:
        ShaderUniformItem(int _value);
        ShaderUniformItem(float _value);
        ShaderUniformItem(const Color& _value);
        ShaderUniformItem(const Vector3& _value);
        ShaderUniformItem(const Matrix4x4& _value);

        void apply(int _id, const char* _name) const;

    private:
        enum itemType {
            INT,
            FLOAT,
            COLOR,
            VECTOR3,
            MATRIX4X4
        };

        itemType type = INT;

        int intValue = 0;
        float floatValue = 0.0f;
        Color colorValue = Color::white();
        Vector3 vector3Value = Vector3::zero();
        Matrix4x4 matrixValue = Matrix4x4::identity();

        void setInt(int _id, const char*& _name) const;
        void setFloat(int _id, const char*& _name) const;
        void setColor(int _id, const char*& _name) const;
        void setVector3(int _id, const char*& _name) const;
        void setMatrix4x4(int _id, const char*& _name) const;
};

// template <typename T>
// class ShaderUniformItem : public UniformItem {
//     public:
//         ShaderUniformItem(const T _value) {
//             value = _value;
//         }
//
//         void apply(int _id, const char* _name) override {
//             std::cout << "Type not supported by ShaderUniformItem!" << std::endl;
//         }
//
//     private:
//         T value {};
// };
//
// template <>
// class ShaderUniformItem<int> : public UniformItem {
//     public:
//         ShaderUniformItem(const int _value) {
//             value = _value;
//         }
//
//         void apply(int _id, const char* _name) override {
//             glUniform1i(glGetUniformLocation(_id, _name), value);
//         }
//
//     private:
//         int value {};
// };
//
// template <>
// class ShaderUniformItem<float> : public UniformItem {
//     public:
//         explicit ShaderUniformItem(const float _value) {
//             value = _value;
//         }
//
//         void apply(const int _id, const char* _name) override {
//             glUniform1f(glGetUniformLocation(_id, _name), value);
//         }
//
//     private:
//         float value {};
// };
//
// template <>
// class ShaderUniformItem<Color> : public UniformItem {
//     public:
//         explicit ShaderUniformItem(const Color& _value) {
//             value = _value;
//         }
//
//         void apply(const int _id, const char* _name) override {
//             glUniform4fv(glGetUniformLocation(_id, _name), 1, &value.r);
//         }
//
//     private:
//         Color value {};
// };
//
// template <>
// class ShaderUniformItem<Vector3> : public UniformItem {
//     public:
//         explicit ShaderUniformItem(const Vector3& _value) {
//             value = _value;
//         }
//
//         void apply(const int _id, const char* _name) override {
//             glUniform3fv(glGetUniformLocation(_id, _name), 1, &value.x);
//         }
//
//     private:
//         Vector3 value {};
// };
//
// template <>
// class ShaderUniformItem<Matrix4x4> : public UniformItem {
//     public:
//         explicit ShaderUniformItem(const Matrix4x4& _value) {
//             value = _value;
//         }
//
//         void apply(const int _id, const char* _name) override {
//             glUniformMatrix4fv(glGetUniformLocation(_id, _name), 1, GL_FALSE, &value.m00);
//         }
//
//     private:
//         Matrix4x4 value {};
// };