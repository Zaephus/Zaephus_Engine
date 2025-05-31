
#include "ModelLoader.h"

#include <iostream>

#include <ZMath.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Color.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"

std::map<std::string, std::vector<Model>> ModelLoader::loadedModels = std::map<std::string, std::vector<Model>>();

std::vector<Model> ModelLoader::load(const primitiveType _type) { return load(_type, false); }
std::vector<Model> ModelLoader::load(primitiveType _type, const bool _loadUnique) {
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

std::vector<Model> ModelLoader::load(const std::string& _fileName) { return load(_fileName, false); }
std::vector<Model> ModelLoader::load(const std::string& _fileName, const bool _loadUnique) {
    const std::string path = "resources/models/" + _fileName;

    std::vector<Model> models;

    if(!_loadUnique && loadedModels.contains(path)) {
        for(size_t i = 0; i < loadedModels.size(); i++) {
            models.emplace_back(loadedModels[path][i]);
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

bool ModelLoader::isSceneValid(const aiScene* _scene, const Assimp::Importer* _importer) {
    if(_scene == nullptr || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || _scene->mRootNode == nullptr) {
        std::cerr << "Assimp error: " << _importer->GetErrorString() << std::endl;
        return false;
    }

    return true;
}

void ModelLoader::processNode(std::vector<Model>* _models, const aiNode* _aiNode, const aiScene* _aiScene) {
    for(size_t i = 0; i < _aiNode->mNumMeshes; i++) {
        const aiMesh* loadedAiMesh = _aiScene->mMeshes[_aiNode->mMeshes[i]];
        Mesh* mesh = processMesh(loadedAiMesh);

        Shader* shader = nullptr;

        if(_aiScene->HasMaterials()) {
            const aiMaterial* loadedMaterial = _aiScene->mMaterials[loadedAiMesh->mMaterialIndex];
            shader = processMaterial(loadedMaterial);
        }

        _models->emplace_back(mesh, shader);
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

Shader* ModelLoader::processMaterial(const aiMaterial* _aiMaterial) {
    aiColor3D color;
    _aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);

    return Shader::diffuseShader({color.r, color.g, color.b, 1.0f});
}

// Texture2D* ModelLoader::loadTexture(const Mesh* _model, const aiMaterial* _material, unsigned int _type) {}