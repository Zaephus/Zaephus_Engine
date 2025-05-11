
#include "ShaderUniformItem.h"

#include <glad/gl.h>
#include <ZMath.h>

#include "Color.h"

void ShaderUniformItem::apply(const int _id, const char*& _name) const {
    if(intValue     != nullptr) { setInt(_id, _name); }
    if(floatValue   != nullptr) { setFloat(_id, _name); }
    if(colorValue   != nullptr) { setColor(_id, _name); }
    if(vector3Value != nullptr) { setVector3(_id, _name); }
    if(matrixValue  != nullptr) { setMatrix4x4(_id, _name); }
}

void ShaderUniformItem::setInt(const int _id, const char*& _name) const {
    glUniform1i(glGetUniformLocation(_id, _name), *intValue);
}

void ShaderUniformItem::setFloat(const int _id, const char*& _name) const {
    glUniform1f(glGetUniformLocation(_id, _name), *floatValue);
}

void ShaderUniformItem::setColor(const int _id, const char*& _name) const {
    glUniform4fv(glGetUniformLocation(_id, _name), 1, &colorValue->r);
}

void ShaderUniformItem::setVector3(const int _id, const char*& _name) const {
    glUniform3fv(glGetUniformLocation(_id, _name), 1, &vector3Value->x);
}

void ShaderUniformItem::setMatrix4x4(const int _id, const char*& _name) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, _name), 1, GL_FALSE, &matrixValue->m00);
}