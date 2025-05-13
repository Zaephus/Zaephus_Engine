
#include "ShaderUniformItem.h"

ShaderUniformItem::ShaderUniformItem(const int _value) {
    intValue = _value;
    type = INT;
}

ShaderUniformItem::ShaderUniformItem(const float _value) {
    floatValue = _value;
    type = FLOAT;
}

ShaderUniformItem::ShaderUniformItem(const Color& _value) {
    colorValue = _value;
    type = COLOR;
}

ShaderUniformItem::ShaderUniformItem(const Vector3& _value) {
    vector3Value = _value;
    type = VECTOR3;
}

ShaderUniformItem::ShaderUniformItem(const Matrix4x4& _value) {
    matrixValue = _value;
    type = MATRIX4X4;
}

void ShaderUniformItem::apply(const int _id, const char* _name) const {
    switch(type) {
        case INT:       setInt(_id, _name); break;
        case FLOAT:     setFloat(_id, _name); break;
        case COLOR:     setColor(_id, _name); break;
        case VECTOR3:   setVector3(_id, _name); break;
        case MATRIX4X4: setMatrix4x4(_id, _name); break;
        default: std::cerr << "Uniform type not supported!" << std::endl;
    }
}

void ShaderUniformItem::setInt(int _id, const char*& _name) const {
    glUniform1i(glGetUniformLocation(_id, _name), intValue);
}

void ShaderUniformItem::setFloat(int _id, const char*& _name) const {
    glUniform1f(glGetUniformLocation(_id, _name), floatValue);
}

void ShaderUniformItem::setColor(int _id, const char*& _name) const {
    glUniform4fv(glGetUniformLocation(_id, _name), 1, &colorValue.r);
}

void ShaderUniformItem::setVector3(int _id, const char*& _name) const {
    glUniform3fv(glGetUniformLocation(_id, _name), 1, &vector3Value.x);
}

void ShaderUniformItem::setMatrix4x4(int _id, const char*& _name) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, _name), 1, GL_FALSE, &matrixValue.m00);
}