
#include "ModelLoader.h"

#include <iostream>
#include <sstream>

#include <ZMath.h>

#include "Color.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "Texture2D.h"

std::map<std::string, std::vector<Model*>> ModelLoader::loadedModels = std::map<std::string, std::vector<Model*>>();

std::vector<Model*> ModelLoader::load(const primitiveType _type) { return load(_type, false); }
std::vector<Model*> ModelLoader::load(const primitiveType _type, const bool _loadUnique) {
    switch(_type) {
        case arrow:    return load("ZE_arrow.obj", _loadUnique);
        case cube:     return load("ZE_cube.obj", _loadUnique);
        case quad:     return load("ZE_quad.obj", _loadUnique);
        case cylinder: return load("ZE_cylinder.obj", _loadUnique);
        case capsule:  return load("ZE_capsule.obj", _loadUnique);
        case sphere:   return load("ZE_sphere.obj", _loadUnique);
        case torus:    return load("ZE_torus.obj", _loadUnique);
        default:
            std::cerr << "Primitive type " << _type << " does not exist" << std::endl;
            return {};
    }
}

std::vector<Model*> ModelLoader::load(const std::string& _fileName) { return load(_fileName, false); }
std::vector<Model*> ModelLoader::load(const std::string& _fileName, const bool _loadUnique) {
    const std::string path = "resources/models/" + _fileName;

    std::vector<Model*> models;

    if(!_loadUnique && loadedModels.contains(path)) {
        for(size_t i = 0; i < loadedModels.size(); i++) {
            models.push_back(loadedModels[path][i]);
        }
        return models;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!isSceneValid(scene, &importer)) {
        return models;
    }

    processNode(&models, scene->mRootNode, scene);

    loadedModels[path] = models;

    return models;
}

void ModelLoader::dispose() {
    for(auto it : loadedModels) {
        for(size_t i = 0; i < it.second.size(); i++) {
            it.second[i]->mesh->destroy();
            it.second[i]->shader->destroy();
        }
    }

    loadedModels.clear();
}

bool ModelLoader::isSceneValid(const aiScene* _scene, const Assimp::Importer* _importer) {
    if(_scene == nullptr || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || _scene->mRootNode == nullptr) {
        std::cerr << "Assimp error: " << _importer->GetErrorString() << std::endl;
        return false;
    }

    return true;
}

void ModelLoader::processNode(std::vector<Model*>* _models, const aiNode* _aiNode, const aiScene* _aiScene) {
    for(size_t i = 0; i < _aiNode->mNumMeshes; i++) {
        const aiMesh* loadedAiMesh = _aiScene->mMeshes[_aiNode->mMeshes[i]];
        Mesh* mesh = processMesh(loadedAiMesh);

        Shader* shader = nullptr;

        if(_aiScene->HasMaterials()) {
            const aiMaterial* loadedMaterial = _aiScene->mMaterials[loadedAiMesh->mMaterialIndex];
            shader = processMaterial(loadedMaterial);
        }

        _models->push_back(new Model(mesh, shader));
    }

    for(size_t i = 0; i < _aiNode->mNumChildren; i++) {
        processNode(_models, _aiNode->mChildren[i], _aiScene);
    }
}

Mesh* ModelLoader::processMesh(const aiMesh* _aiMesh) {
    Mesh* mesh = new Mesh();

    for(size_t i = 0; i < _aiMesh->mNumVertices; i++) {
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

    for(size_t i = 0; i < _aiMesh->mNumFaces; i++) {
        const aiFace face = _aiMesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; j++) {
            mesh->indices.push_back(face.mIndices[j]);
        }
    }

    return mesh;
}

Shader* ModelLoader::processMaterial(const aiMaterial* _mat) {
    float shininess;
    _mat->Get(AI_MATKEY_SHININESS, shininess);

    if(shininess <= 0) {
        shininess = 1.0f;
    }

    if(_mat->GetTextureCount(aiTextureType_DIFFUSE) >= 1) {
        Texture2D* diffuseTex = loadTexture(_mat, aiTextureType_DIFFUSE);
        Texture2D* specularTex = nullptr;

        if(_mat->GetTextureCount(aiTextureType_SPECULAR) >= 1) {
            specularTex = loadTexture(_mat, aiTextureType_SPECULAR);
        }
        else {
            specularTex = Texture2D::load(Color::white());
        }

        return Shader::diffuseTextureShader(diffuseTex, specularTex, shininess);
    }

    aiColor3D color;
    _mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);

    return Shader::diffuseShader({color.r, color.g, color.b, 1.0f}, shininess);
}

Texture2D* ModelLoader::loadTexture(const aiMaterial* _mat, unsigned int _type) {
    aiString aiPath;
    _mat->GetTexture(static_cast<aiTextureType>(_type), 0, &aiPath);

    std::stringstream stream(std::string(aiPath.C_Str()));
    std::string fileName;

    while(!stream.eof()) {
        std::getline(stream, fileName, '/');
    }

    return Texture2D::load(fileName);
}