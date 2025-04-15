
#include "MeshRenderer.h"

#include <iostream>
#include <string>

#include <ZMath.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <Camera.h>
#include "Color.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Transform.h"

std::vector<Mesh*> MeshRenderer::loadedMeshes = std::vector<Mesh*>();

Action<void(MeshRenderer*)> MeshRenderer::modelCreatedCall = Action<void(MeshRenderer*)>();
Action<void(MeshRenderer*)> MeshRenderer::modelDestroyedCall = Action<void(MeshRenderer*)>();

MeshRenderer::~MeshRenderer() {
    modelDestroyedCall.invoke(this);

    for(const Mesh* mesh : meshes) {
        delete mesh;
    }
}

void MeshRenderer::render() const {
    const Matrix4x4 modelMatrix = transform->objectMatrix();
    shader->setMatrix4x4("modelMatrix", modelMatrix);

    const Matrix4x4 normalMatrix = modelMatrix.inverse().transposed();
    shader->setMatrix4x4("normalMatrix", normalMatrix);

    if(Camera::activeCam->transform->hasChanged) {
        shader->setMatrix4x4("viewMatrix", Camera::activeCam->viewMatrix());
    }
    if(Camera::activeCam->projectionChanged) {
        shader->setMatrix4x4("projectionMatrix", Camera::activeCam->projectionMatrix);
    }

    for(Mesh* mesh : meshes) {
        mesh->render();
    }
}

void MeshRenderer::setShader(Shader* _shader) {
    shader = _shader;
}

void MeshRenderer::setBool(const std::string &name, const bool value) const {
    shader->setBool(name, value);
}
void MeshRenderer::setInt(const std::string &name, const int value) const {
    shader->setInt(name, value);
}
void MeshRenderer::setFloat(const std::string &name, const float value) const {
    shader->setFloat(name, value);
}

void MeshRenderer::setColor(const std::string &name, const float r, const float g, const float b, const float a) const {
    shader->setColor(name, r, g, b, a);
}
void MeshRenderer::setColor(const std::string &name, const Color& color) const {
    shader->setColor(name, color);
}

void MeshRenderer::setVector3(const std::string &name, const float x, const float y, const float z) const {
    shader->setVector3(name, x, y, z);
}
void MeshRenderer::setVector3(const std::string &name, const Vector3& vector) const {
    shader->setVector3(name, vector);
}

void MeshRenderer::setMatrix4x4(const std::string &name, const Matrix4x4& matrix) const {
    shader->setMatrix4x4(name, matrix);
}
void MeshRenderer::setTexture2D(const std::string &name, Texture2D* texture) const {
    shader->setTexture2D(name, texture);
}

void MeshRenderer::setLight(const std::string &name, const Light* light) const {
    shader->setLight(name, light);
}

bool MeshRenderer::isTransparent() const {
    return shader->order == Shader::transparents;
}


MeshRenderer* MeshRenderer::loadModel(const primitiveType _type, Shader* _overrideShader) {
    MeshRenderer* renderer = nullptr;
    switch(_type) {
        case cube:
            renderer = loadModel("cube.obj", _overrideShader);
            break;
        case quad:
            renderer = loadModel("quad.obj", _overrideShader);
            break;
        case cylinder:
            renderer = loadModel("cylinder.obj", _overrideShader);
            break;
        case capsule:
            renderer = loadModel("capsule.obj", _overrideShader);
            break;
        case sphere:
            renderer = loadModel("sphere.obj", _overrideShader);
            break;
        case torus:
            renderer = loadModel("torus.obj", _overrideShader);
            break;
        default:
            std::cerr << "Primitive type " << _type << " does not exist" << std::endl;
            break;
    }

    return renderer;
}

MeshRenderer* MeshRenderer::loadModel(const std::string& _fileName, Shader* _overrideShader) {
    const std::string path = "resources/models/" + _fileName;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    MeshRenderer* model = new MeshRenderer();
    model->directory = path.substr(0, path.find_last_of('/'));

    model->shader = _overrideShader;

    processNode(model, scene->mRootNode, scene);

    for(Mesh* mesh : model->meshes) {
        mesh->initialize();
    }

    modelCreatedCall.invoke(model);
    return model;
}

void MeshRenderer::processNode(MeshRenderer* _model, const aiNode* _aiNode, const aiScene* _aiScene) { // NOLINT(*-no-recursion)
    for(unsigned int i = 0; i < _aiNode->mNumMeshes; i++) {
        const aiMesh* mesh = _aiScene->mMeshes[_aiNode->mMeshes[i]];
        processMesh(_model, mesh, _aiScene);
    }
    for(unsigned int i = 0; i < _aiNode->mNumChildren; i++) {
        processNode(_model, _aiNode->mChildren[i], _aiScene);
    }
}

void MeshRenderer::processMesh(MeshRenderer* _model, const aiMesh* _aiMesh, const aiScene* _aiScene) {
    for(size_t i = 0; i < loadedMeshes.size(); i++) {
        if(*loadedMeshes[i] == *_aiMesh) {
            _model->meshes.push_back(loadedMeshes[i]);
            return;
        }
    }

    Mesh* mesh = new Mesh();

    for(unsigned int i = 0; i < _aiMesh->mNumVertices; i++) {
        mesh->positions.emplace_back(
            _aiMesh->mVertices[i].x,
            _aiMesh->mVertices[i].y,
            _aiMesh->mVertices[i].z
        );

        if(_aiMesh->HasVertexColors(0)) {
            mesh->colors.emplace_back(
                _aiMesh->mColors[0][i].r,
                _aiMesh->mColors[0][i].g,
                _aiMesh->mColors[0][i].b,
                _aiMesh->mColors[0][i].a
            );
        }

        if(_aiMesh->HasTextureCoords(0)) {
            mesh->uvs.emplace_back(
                _aiMesh->mTextureCoords[0][i].x,
                _aiMesh->mTextureCoords[0][i].y
            );
        }

        if(_aiMesh->HasNormals()) {
            mesh->normals.emplace_back(
                _aiMesh->mNormals[i].x,
                _aiMesh->mNormals[i].y,
                _aiMesh->mNormals[i].z
            );
        }
    }

    for(unsigned int i = 0; i < _aiMesh->mNumFaces; i++) {
        const aiFace face = _aiMesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            mesh->indices.emplace_back(face.mIndices[j]);
        }
    }

    if(_model->shader == nullptr) {
        const aiMaterial* material = _aiScene->mMaterials[_aiMesh->mMaterialIndex];

        if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
// TODO: add texture loading support back in
//            mesh->shader = Shader::textureShader(loadTexture(_model, material, aiTextureType_DIFFUSE));
        }
        else {
            aiColor3D color(0.0f, 0.0f, 0.0f);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            _model->shader = Shader::diffuseShader({color.r, color.g, color.b, 1.0f});
        }
    }

    _model->meshes.push_back(mesh);
    loadedMeshes.push_back(mesh);
}

Texture2D* MeshRenderer::loadTexture(const MeshRenderer* _model, const aiMaterial* _material, const aiTextureType _type) {
    aiString aiPath;
    _material->GetTexture(_type, 0, &aiPath);

    const std::string path = _model->directory + "/" + aiPath.C_Str();

    Texture2D* texture = new Texture2D();
    texture->flipVerticallyOnLoad = true;
    Texture2D::load(texture, path);

    return texture;
}