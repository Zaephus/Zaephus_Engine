
#include "Shader.h"

#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <cstring>

#include <glad/gl.h>

#include "Color.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Renderer.h"
#include "Scene.h"
#include "ShaderUniformItem.h"
#include "Texture2D.h"
#include "Transform.h"

#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

const std::string Shader::shaderVersion = "#version 330 core";

Shader* Shader::activeShader = nullptr;

Shader::Shader(const std::string& _fragmentPath) : Shader("ZE_Base.vert", _fragmentPath) {}

Shader::Shader(const std::string& _vertexPath, const std::string& _fragmentPath) {
    vertexPath = _vertexPath;
    fragmentPath = _fragmentPath;
}

void Shader::initialize() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Shader::Initialize",0x006303);
#endif

    if(id != 0) { return; }

    const std::string vertexCode = processVertexCode(vertexPath);
    const unsigned int vertexShader = compile(vertexCode, GL_VERTEX_SHADER);

    const std::string fragmentCode = processFragmentCode(fragmentPath);
    const unsigned int fragmentShader = compile(fragmentCode, GL_FRAGMENT_SHADER);

    id = createProgram(vertexShader, fragmentShader);

    for(Texture2D* boundTexture : boundTextures) {
        boundTexture->initialize();
    }

    Renderer::cleanupRenderObjectsCall.bind<Shader, &Shader::internalDestroy>(this);
}

void Shader::destroy() {
    Renderer::destroyRenderObjectCall.bind<Shader, &Shader::internalDestroy>(this);
}

void Shader::internalDestroy() {
    Renderer::destroyRenderObjectCall.unbind<Shader, &Shader::internalDestroy>(this);
    Renderer::cleanupRenderObjectsCall.unbind<Shader, &Shader::internalDestroy>(this);

    for(size_t i = 0; i < boundTextures.size(); i++) {
        boundTextures[i]->destroy();
    }

    glUseProgram(id);
    glDeleteProgram(id);

    delete this;
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

    if(drawAsWireframe && polygonMode[0] == GL_FILL) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if(!drawAsWireframe && polygonMode[0] == GL_LINE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    for(const Texture2D* boundTexture : boundTextures) {
        boundTexture->use();
    }
}

void Shader::applyUniforms() {
#ifdef ENABLE_PROFILING
    ZoneScopedNC("Shader::ApplyUniforms", 0x006303);
#endif

    for(auto& [_name, _item] : uniformQueue) {
        _item.apply(static_cast<int>(id), _name.c_str());
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
            boundTextures.erase(boundTextures.begin() + i);
            break;
        }
    }

    boundTextures.push_back(_texture);
    _texture->setUnit(textureIndex);
    _texture->boundUniform = _name;
}

void Shader::setDirLight(const std::string& _name, const DirectionalLight* _light) {
    setVector3(_name + ".direction", _light->transform->forward());

    setColor(_name + ".color", _light->color);

    setFloat(_name + ".intensity", _light->intensity);
    setFloat(_name + ".ambientStrength", _light->ambientStrength);
    setFloat(_name + ".specularStrength", _light->specularStrength);
}

void Shader::setPointLight(const std::string& _name, const PointLight* _light) {
    setVector3(_name + ".position", _light->transform->position);

    setColor(_name + ".color", _light->color);

    setFloat(_name + ".intensity", _light->intensity);
    setFloat(_name + ".ambientStrength", _light->ambientStrength);
    setFloat(_name + ".specularStrength", _light->specularStrength);

    setFloat(_name + ".constant", _light->constantAtt);
    setFloat(_name + ".linear", _light->linearAtt);
    setFloat(_name + ".quadratic", _light->quadraticAtt);
}

bool Shader::isTransparent() const {
    return order == transparents;
}

Shader *Shader::unlitShader(const float _r, const float _g, const float _b, const float _a) {
    return unlitShader({ _r, _g, _b, _a } );
}
Shader* Shader::unlitShader(const Color& _c) {
    Shader* shader = new Shader("ZE_Base.vert", "ZE_Unlit.frag");
    shader->setColor("material.color", _c);

    return shader;
}

Shader* Shader::unlitTextureShader(const std::string& _texturePath, const float _uvScale) {
    Texture2D* diffuse = Texture2D::load(_texturePath);
    return unlitTextureShader(diffuse, _uvScale);
}
Shader* Shader::unlitTextureShader(Texture2D* _texture, const float _uvScale) {
    Shader* shader = new Shader("ZE_Base.vert", "ZE_UnlitTexture.frag");
    shader->setTexture2D("material.texture", _texture);
    shader->setFloat("material.uvScale", _uvScale);

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
    Shader* shader = new Shader("ZE_Base.vert", "ZE_Diffuse.frag");

    if(_c.a < 1.0f) { shader->order = transparents; }
    else { shader->order = opaques; }

    shader->setColor("material.color", _c);
    shader->setFloat("material.shininess", _shininess);

    return shader;
}

Shader* Shader::diffuseTextureShader(const std::string& _diffusePath, float _uvScale, float _shininess) {
    Texture2D* diffuse = Texture2D::load(_diffusePath);
    Texture2D* specular = Texture2D::load(Color::grey());

    return diffuseTextureShader(diffuse, specular, _uvScale, _shininess);
}
Shader* Shader::diffuseTextureShader(const std::string& _diffusePath, const std::string& _specularPath, const float _uvScale, const float _shininess) {
    Texture2D* diffuse = Texture2D::load(_diffusePath);
    Texture2D* specular = Texture2D::load(_specularPath);

    return diffuseTextureShader(diffuse, specular, _uvScale, _shininess);
}
Shader* Shader::diffuseTextureShader(Texture2D* _diffuse, float _uvScale, float _shininess) {
    return diffuseTextureShader(_diffuse, Texture2D::load(Color::grey()), _uvScale, _shininess);
}
Shader* Shader::diffuseTextureShader(Texture2D* _diffuse, Texture2D* _specular, const float _uvScale, const float _shininess) {
    Shader* shader = new Shader("ZE_Base.vert", "ZE_DiffuseTexture.frag");

    shader->setTexture2D("material.specular", _specular);
    shader->setTexture2D("material.diffuse", _diffuse);
    shader->setFloat("material.uvScale", _uvScale);
    shader->setFloat("material.shininess", _shininess);

    return shader;
}

Shader *Shader::instancedUnlitShader(const float _r, const float _g, const float _b, const float _a) {
    return instancedUnlitShader({ _r, _g, _b, _a } );
}
Shader* Shader::instancedUnlitShader(const Color& _c) {
    Shader* shader = new Shader("ZE_Instanced.vert", "ZE_Unlit.frag");
    shader->setColor("material.color", _c);

    return shader;
}

Shader* Shader::instancedUnlitTextureShader(const std::string& _texturePath, const float _uvScale) {
    Texture2D* diffuse = Texture2D::load(_texturePath);
    return instancedUnlitTextureShader(diffuse, _uvScale);
}
Shader* Shader::instancedUnlitTextureShader(Texture2D* _texture, const float _uvScale) {
    Shader* shader = new Shader("ZE_Instanced.vert", "ZE_UnlitTexture.frag");
    shader->setTexture2D("material.texture", _texture);
    shader->setFloat("material.uvScale", _uvScale);

    return shader;
}

Shader* Shader::instancedDiffuseShader(const float _r, const float _g, const float _b, const float _a) {
    return instancedDiffuseShader({ _r, _g, _b, _a });
}
Shader* Shader::instancedDiffuseShader(const float _r, const float _g, const float _b, const float _a, const float _shininess) {
    return instancedDiffuseShader({ _r, _g, _b, _a }, _shininess);
}
Shader* Shader::instancedDiffuseShader(const Color& _c) {
    return instancedDiffuseShader(_c, 32.0f);
}
Shader* Shader::instancedDiffuseShader(const Color& _c, const float _shininess) {
    Shader* shader = new Shader("ZE_Instanced.vert", "ZE_Diffuse.frag");

    if(_c.a < 1.0f) { shader->order = transparents; }
    else { shader->order = opaques; }

    shader->setColor("material.color", _c);
    shader->setFloat("material.shininess", _shininess);

    return shader;
}

Shader* Shader::instancedDiffuseTextureShader(const std::string& _diffusePath, const std::string& _specularPath, const float _uvScale, const float _shininess) {
    Texture2D* diffuse = Texture2D::load(_diffusePath);
    Texture2D* specular = Texture2D::load(_specularPath);

    return instancedDiffuseTextureShader(diffuse, specular, _uvScale, _shininess);
}
Shader* Shader::instancedDiffuseTextureShader(Texture2D* _diffuse, Texture2D* _specular, const float _uvScale, const float _shininess) {
    Shader* shader = new Shader("ZE_Instanced.vert", "ZE_DiffuseTexture.frag");

    shader->setTexture2D("material.diffuse", _diffuse);
    shader->setTexture2D("material.specular", _specular);
    shader->setFloat("material.uvScale", _uvScale);
    shader->setFloat("material.shininess", _shininess);

    return shader;
}

std::string Shader::processVertexCode(const std::string& _vertexPath) {
    const std::string vertexCode = load(_vertexPath);
    std::vector<std::string> vertLines = convertToLines(vertexCode);

    if(std::strstr(vertLines[0].c_str(), "#version") == nullptr) {
        vertLines.insert(vertLines.begin(), shaderVersion + "\n");
    }

    const std::string globalShaderVars = load("GlobalVars.shard");
    const std::string vertexLayout = load("VertLayout.shard");
    const std::string vertexOutVars = load("VertOutVars.shard");
    const std::string vertexUniforms = load("VertUniforms.shard");
    const std::string vertShards = globalShaderVars + "\n" + vertexLayout + "\n" + vertexOutVars + "\n" + vertexUniforms + "\n";

    vertLines.insert(vertLines.begin()+1, vertShards);

    std::string finalVertexCode;
    for(size_t i = 0; i < vertLines.size(); i++) {
        finalVertexCode += vertLines[i];
    }
    return finalVertexCode;
}

std::string Shader::processFragmentCode(const std::string& _fragmentPath) {
    const std::string fragmentCode = load(_fragmentPath);
    std::vector<std::string> fragLines = convertToLines(fragmentCode);

    if(std::strstr(fragLines[0].c_str(), "#version") == nullptr) {
        fragLines.insert(fragLines.begin(), shaderVersion + "\n");
    }

    const std::string globalShaderVars = load("GlobalVars.shard");
    const std::string fragOutVars = load("FragOutVars.shard");
    const std::string fragInVars = load("FragInVars.shard");
    const std::string lightingData = load("LightingData.shard");
    const std::string lightingFunctions = load("LightingFunctions.shard");
    const std::string fragShards = globalShaderVars + "\n" + fragOutVars + "\n" + fragInVars + "\n" + lightingData + "\n" + lightingFunctions + "\n";

    fragLines.insert(fragLines.begin()+1, fragShards);

    std::string finalFragmentCode;
    for(size_t i = 0; i < fragLines.size(); i++) {
        finalFragmentCode += fragLines[i];
    }

    return finalFragmentCode;
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
        int maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char errorLog[maxLength];
        glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);
        std::string type = _shaderType == GL_FRAGMENT_SHADER ? "fragment" : "vertex";
        throw std::runtime_error(std::format("Shader {0}, {1}, compilation failed:\n {2}", type, type == "fragment" ? fragmentPath : vertexPath, std::string(errorLog)));
    }

    return shader;
}

unsigned int Shader::createProgram(const unsigned int& _vertexShader, const unsigned int& _fragmentShader) {
    const unsigned int programID = glCreateProgram();
    glAttachShader(programID, _vertexShader);
    glAttachShader(programID, _fragmentShader);
    glLinkProgram(programID);

    int success;

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        throw std::runtime_error(std::format("Shader program {0}, {1} linking failed:\n {2}", vertexPath, fragmentPath, infoLog));
    }

    glDeleteShader(_vertexShader);
    glDeleteShader(_fragmentShader);

    return programID;
}

std::vector<std::string> Shader::convertToLines(const std::string& _fileText) {
    std::vector<std::string> lines;
    std::stringstream ss(_fileText);
    std::string line;
    while(std::getline(ss, line, '\n')) {
        if(!line.empty()) {
            lines.emplace_back(line + "\n");
        }
    }

    return lines;
}