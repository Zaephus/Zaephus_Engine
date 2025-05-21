
#include "Shader.h"

#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

#include <ZMath.h>
#include <glad/gl.h>

#include "Color.h"
#include "Light.h"
#include "ShaderUniformItem.h"
#include "Texture2D.h"
#include "Transform.h"

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

Shader* Shader::activeShader = nullptr;

Shader::Shader(const char* _fragmentPath) : Shader("BaseVertex.glsl", _fragmentPath) {}

Shader::Shader(const char* _vertexPath, const char* _fragmentPath) {
    vertexPath = _vertexPath;
    fragmentPath = _fragmentPath;
}

void Shader::initialize() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Shader::Initialize",0x006303);
#endif


    if(id != 0) { return; }
    const std::string vertexCode = load(vertexPath);
    const std::string fragmentCode = load(fragmentPath);

    const unsigned int vertexShader = compile(vertexCode, GL_VERTEX_SHADER);
    const unsigned int fragmentShader = compile(fragmentCode, GL_FRAGMENT_SHADER);

    id = createProgram(vertexShader, fragmentShader);
}

void Shader::destroy() const {
    for(const Texture2D* boundTexture : boundTextures) {
        boundTexture->destroy();
        delete boundTexture;
    }

    glUseProgram(id);
    glDeleteProgram(id);
}

void Shader::bind() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Shader::Bind",0x006303);
#endif

    if(activeShader == this) { return; }

    activeShader = this;

    glUseProgram(id);

    if(glIsEnabled(GL_DEPTH_TEST) && !depthTestEnabled) { glDisable(GL_DEPTH_TEST); }
    else if(!glIsEnabled(GL_DEPTH_TEST) && depthTestEnabled) { glEnable(GL_DEPTH_TEST); }

    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);

    if(drawAsWireframe && polygonMode[1] == GL_FILL) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if(!drawAsWireframe && polygonMode[1] == GL_LINE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    for(const Texture2D* boundTexture : boundTextures) {
        boundTexture->use();
    }
}

void Shader::applyUniforms() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Shader::ApplyUniforms",0x006303);
#endif

    for(auto& [_name, _item] : uniformQueue) {
        _item.apply(id, _name.c_str());
    }
    uniformQueue.clear();
}

void Shader::setBool(const std::string &_name, const bool _value) { setInt(_name, _value); }
void Shader::setInt(const std::string &_name, const int _value) {
    uniformQueue.emplace(std::make_pair<std::string, ShaderUniformItem>(_name.data(), {_value}));
}
void Shader::setFloat(const std::string &_name, const float _value) {
    uniformQueue.emplace(std::make_pair<std::string, ShaderUniformItem>(_name.data(), {_value}));
}

void Shader::setColor(const std::string& _name, const float _r, const float _g, const float _b, const float _a) {
    setColor(_name, { _r, _g, _b, _a });
}
void Shader::setColor(const std::string& _name, const Color& _value) {
    assignedColors[_name] = _value;
    uniformQueue.emplace(std::make_pair<std::string, ShaderUniformItem>(_name.data(), {_value}));
}

Color Shader::getColor(const std::string &_name) const {
    return assignedColors.at(_name);
}

void Shader::setVector3(const std::string& _name, const float _x, const float _y, const float _z) {
    setVector3(_name, { _x, _y, _z });
}
void Shader::setVector3(const std::string& _name, const Vector3& _value) {
    uniformQueue.emplace(std::make_pair<std::string, ShaderUniformItem>(_name.data(), {_value}));
}

void Shader::setMatrix4x4(const std::string &_name, const Matrix4x4& _value) {
    uniformQueue.emplace(std::make_pair<std::string, ShaderUniformItem>(_name.data(), {_value}));
}

void Shader::setTexture2D(const std::string &_name, Texture2D* _texture) {
    const int textureIndex = static_cast<int>(boundTextures.size());
    setInt(_name, textureIndex);

    for(int i = 0; i < boundTextures.size(); i++) {
        if(boundTextures[i]->boundUniform == _name) {
            boundTextures[i]->destroy();
            std::cout << "Old texture was destroyed" << std::endl;
            boundTextures.erase(boundTextures.begin() + i);
            break;
        }
    }

    boundTextures.push_back(_texture);
    _texture->setUnit(textureIndex);
    _texture->boundUniform = _name;
}

void Shader::setLight(const std::string& _name, const Light* _light) {
    setVector3(_name + ".position", _light->transform->position);

    setColor(_name + ".color", _light->color);
    setFloat(_name + ".ambientStrength", _light->ambientStrength);
    setFloat(_name + ".specularStrength", _light->specularStrength);

    // glUniform3fv(glGetUniformLocation(id, (_name + ".position").c_str()), 1, &_light->transform->position.x);

    // glUniform4fv(glGetUniformLocation(id, (_name + ".color").c_str()), 1, &_light->color.r);
    // glUniform1f(glGetUniformLocation(id, (_name + ".ambientStrength").c_str()), _light->ambientStrength);
    // glUniform1f(glGetUniformLocation(id, (_name + ".specularStrength").c_str()), _light->specularStrength);
}

bool Shader::isTransparent() const {
    return order == transparents;
}

Shader *Shader::unlitShader(const float _r, const float _g, const float _b, const float _a) {
    return unlitShader({ _r, _g, _b, _a } );
}


Shader* Shader::unlitShader(const Color& _c) {
    Shader* shader = new Shader("BaseVertex.glsl", "UnlitFragment.glsl");

    shader->setColor("objectColor", _c);

    return shader;
}

Shader* Shader::diffuseShader(const float _r, const float _g, const float _b, const float _a) {
    return diffuseShader({ _r, _g, _b, _a });
}
Shader* Shader::diffuseShader(const float _r, const float _g, const float _b, const float _a, const float _shininess) {
    return diffuseShader({ _r, _g, _b, _a }, _shininess);
}
Shader* Shader::diffuseShader(const Color& _c) {
    return diffuseShader(_c, 32.0f);
}
Shader* Shader::diffuseShader(const Color& _c, const float _shininess) {
    Shader* shader = new Shader("BaseVertex.glsl", "DiffuseFragment.glsl");

    if(_c.a < 1.0f) { shader->order = transparents; }
    else { shader->order = opaques; }

    shader->setColor("material.color", _c);
    shader->setFloat("material.shininess", _shininess);

    return shader;
}

Shader* Shader::instancedDiffuseShader(const float _r, const float _g, const float _b, const float _a) {
    return diffuseShader({ _r, _g, _b, _a });
}
Shader* Shader::instancedDiffuseShader(const float _r, const float _g, const float _b, const float _a, const float _shininess) {
    return diffuseShader({ _r, _g, _b, _a }, _shininess);
}
Shader* Shader::instancedDiffuseShader(const Color& _c) {
    return diffuseShader(_c, 32.0f);
}
Shader* Shader::instancedDiffuseShader(const Color& _c, const float _shininess) {
    Shader* shader = new Shader("InstancedVertex.glsl", "DiffuseFragment.glsl");

    if(_c.a < 1.0f) { shader->order = transparents; }
    else { shader->order = opaques; }

    shader->setColor("material.color", _c);
    shader->setFloat("material.shininess", _shininess);

    return shader;
}

Shader* Shader::textureShader(const std::string& _diffusePath, const std::string& _specularPath, const float _shininess) {
    Texture2D* diffuse = new Texture2D();
    Texture2D::load(diffuse, _diffusePath);

    Texture2D* specular = new Texture2D();
    Texture2D::load(specular, _specularPath);

    return textureShader(diffuse, specular, _shininess);
}

Shader* Shader::textureShader(Texture2D* _diffuse, Texture2D* _specular, const float _shininess) {
    Shader* shader = new Shader("BaseVertex.glsl", "TextureFrag_ment.glsl");

    shader->setTexture2D("material.diffuse", _diffuse);
    shader->setTexture2D("material.specular", _specular);
    shader->setFloat("material.shininess", _shininess);

    return shader;
}

std::string Shader::load(const std::string& _fileName) {
    const std::string path = "resources/shaders/" + _fileName;

    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        return stream.str();
    }
    catch(std::ifstream::failure& e) {
        std::cerr << "Error in reading shader: " << e.what() << std::endl;
    }
    return "";
}

unsigned int Shader::compile(const std::string& _code, const GLenum _shaderType) {
    const char* shaderCode = _code.c_str();

    const unsigned int shader = glCreateShader(_shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    int success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string type = _shaderType == GL_FRAGMENT_SHADER ? "fragment" : "vertex";
        throw std::runtime_error(std::format("Shader {0} compilation failed:\n {1}", type,infoLog));
    }

    return shader;
}

unsigned int Shader::createProgram(const unsigned int& _vertexShader, const unsigned int& _fragmentShader) {
    const unsigned int id = glCreateProgram();
    glAttachShader(id, _vertexShader);
    glAttachShader(id, _fragmentShader);
    glLinkProgram(id);

    int success;

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        throw std::runtime_error(std::format("Shader program linking failed:\n {0}", infoLog));
    }

    glDeleteShader(_vertexShader);
    glDeleteShader(_fragmentShader);

    return id;
}