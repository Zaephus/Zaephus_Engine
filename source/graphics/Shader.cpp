
#include "Shader.h"

#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

#include <ZMath.h>

#include "Color.h"
#include "Light.h"
#include "Texture2D.h"
#include "Transform.h"

Shader* Shader::activeShader = nullptr;

Shader::Shader(const char* _fragmentPath) : Shader("BaseVertex.glsl", _fragmentPath) {}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    const std::string vertexCode = load(vertexPath);
    const std::string fragmentCode = load(fragmentPath);

    const unsigned int vertexShader = compile(vertexCode, GL_VERTEX_SHADER);
    const unsigned int fragmentShader = compile(fragmentCode, GL_FRAGMENT_SHADER);

    id = createProgram(vertexShader, fragmentShader);
}

Shader::~Shader() {
    for(Texture2D* boundTexture : boundTextures) {
        boundTexture->destroy();
        delete boundTexture;
    }

    glUseProgram(id);
    glDeleteProgram(id);
}

void Shader::use() {
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

void Shader::setBool(const std::string &name, const bool value) {
    use();

    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}
void Shader::setInt(const std::string &name, const int value) {
    use();

    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, const float value) {
    use();

    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setColor(const std::string& name, const float r, const float g, const float b, const float a) {
    const Color c = { r, g, b, a };
    setColor(name, c);
}
void Shader::setColor(const std::string& name, const Color& color) {
    use();

    assignedColors[name] = color;
    const float* colorPtr = &color.r;
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, colorPtr);
}

Color Shader::getColor(const std::string &name) {
    return assignedColors.at(name);
}

void Shader::setVector3(const std::string& name, const float x, const float y, const float z) {
    const Vector3 v = { x, y, z };
    setVector3(name, v);
}
void Shader::setVector3(const std::string& name, const Vector3& vector) {
    use();

    const float* vectorPtr = &vector.x;
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, vectorPtr);
}

void Shader::setMatrix4x4(const std::string &name, const Matrix4x4& matrix) {
    use();

    const int location = glGetUniformLocation(id, name.c_str());
    const float* matrixPtr = &matrix.m00;
    glUniformMatrix4fv(location, 1, GL_TRUE, matrixPtr);
}

void Shader::setTexture2D(const std::string &name, Texture2D* texture) {
    use();

    const int textureIndex = static_cast<int>(boundTextures.size());
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(textureIndex));

    for(int i = 0; i < boundTextures.size(); i++) {
        if(boundTextures[i]->boundUniform == name) {
            boundTextures[i]->destroy();
            std::cout << "Old texture was destroyed" << std::endl;
            boundTextures.erase(boundTextures.begin() + i);
            break;
        }
    }

    boundTextures.push_back(texture);
    texture->setUnit(textureIndex);
    texture->boundUniform = name;
}

void Shader::setLight(const std::string& name, const Light* light) {
    use();

    glUniform3fv(glGetUniformLocation(id, (name + ".position").c_str()), 1, &light->transform->position.x);

    glUniform4fv(glGetUniformLocation(id, (name + ".color").c_str()), 1, &light->color.r);
    glUniform1f(glGetUniformLocation(id, (name + ".ambientStrength").c_str()), light->ambientStrength);
    glUniform1f(glGetUniformLocation(id, (name + ".specularStrength").c_str()), light->specularStrength);
}

bool Shader::isTransparent() const {
    return order == transparents;
}

Shader *Shader::unlitShader(const float r, const float g, const float b, const float a) {
    return unlitShader({ r, g, b, a } );
}


Shader* Shader::unlitShader(const Color& c) {
    Shader* shader = new Shader("BaseVertex.glsl", "UnlitFragment.glsl");
    shader->use();

    shader->setColor("objectColor", c);

    return shader;
}

Shader* Shader::diffuseShader(const float r, const float g, const float b, const float a) {
    return diffuseShader({ r, g, b, a });
}
Shader* Shader::diffuseShader(const float r, const float g, const float b, const float a, const float shininess) {
    return diffuseShader({ r, g, b, a }, shininess);
}
Shader* Shader::diffuseShader(const Color& c) {
    return diffuseShader(c, 32.0f);
}
Shader* Shader::diffuseShader(const Color& c, const float shininess) {
    Shader* shader = new Shader("BaseVertex.glsl", "DiffuseFragment.glsl");
    shader->use();

    if(c.a < 1.0f) { shader->order = transparents; }
    else { shader->order = opaques; }

    shader->setColor("material.color", c);
    shader->setFloat("material.shininess", shininess);

    return shader;
}

Shader* Shader::textureShader(const std::string& diffusePath, const std::string& specularPath, const float shininess) {
    Texture2D* diffuse = new Texture2D();
    Texture2D::load(diffuse, diffusePath);

    Texture2D* specular = new Texture2D();
    Texture2D::load(specular, specularPath);

    return textureShader(diffuse, specular, shininess);
}

Shader* Shader::textureShader(Texture2D* diffuse, Texture2D* specular, const float shininess) {
    Shader* shader = new Shader("BaseVertex.glsl", "TextureFragment.glsl");
    shader->use();

    shader->setTexture2D("material.diffuse", diffuse);
    shader->setTexture2D("material.specular", specular);
    shader->setFloat("material.shininess", shininess);

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

unsigned int Shader::compile(const std::string& code, const GLenum shaderType) {
    const char* shaderCode = code.c_str();

    const unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    int success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string type = shaderType == GL_FRAGMENT_SHADER ? "fragment" : "vertex";
        throw std::runtime_error(std::format("Shader {0} compilation failed:\n {1}", type,infoLog));
    }

    return shader;
}

unsigned int Shader::createProgram(const unsigned int& vertexShader, const unsigned int& fragmentShader) {
    const unsigned int id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    int success;

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        throw std::runtime_error(std::format("Shader program linking failed:\n {0}", infoLog));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return id;
}