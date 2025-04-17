
#include "ModelLoader.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <ZMath.h>

#include <Color.h>
#include <Mesh.h>
#include <MeshRenderer.h>
#include <Shader.h>

std::map<std::string, std::vector<MeshRenderer*>> ModelLoader::loadedModels = std::map<std::string, std::vector<MeshRenderer*>>();

std::vector<MeshRenderer*> ModelLoader::load(const primitiveType _type) {
    switch(_type) {
    case cube:     return load("cube.obj");
    case quad:     return load("quad.obj");
    case cylinder: return load("cylinder.obj");
    case capsule:  return load("capsule.obj");
    case sphere:   return load("sphere.obj");
    case torus:    return load("torus.obj");
    default:
        std::cerr << "Primitive type " << _type << " does not exist" << std::endl;
        return std::vector<MeshRenderer*>();
    }
}

std::vector<MeshRenderer*> ModelLoader::load(const std::string& _fileName) {
    const std::string path = "resources/models/" + _fileName;

    std::vector<MeshRenderer*> renderers;

    if(loadedModels.contains(path)) {
        for(size_t i = 0; i < loadedModels.size(); i++) {
            renderers.push_back(new MeshRenderer(loadedModels[path][i]->getMesh()));
        }
        return renderers;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!isSceneValid(scene, &importer)) {
        return renderers;
    }

    processNode(&renderers, scene->mRootNode, scene);

    loadedModels[path] = renderers;

    return renderers;
}

bool ModelLoader::isSceneValid(const aiScene* _scene, const Assimp::Importer* _importer) {
    if(_scene == nullptr || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || _scene->mRootNode == nullptr) {
        std::cerr << "Assimp error: " << _importer->GetErrorString() << std::endl;
        return false;
    }

    return true;
}

void ModelLoader::processNode(std::vector<MeshRenderer*>* _meshes, const aiNode* _aiNode, const aiScene* _aiScene) {
    for(size_t i = 0; i < _aiNode->mNumMeshes; i++) {
        const aiMesh* loadedAiMesh = _aiScene->mMeshes[_aiNode->mMeshes[i]];
        Mesh* mesh = processMesh(loadedAiMesh);
        std::cout << mesh->positions.size() << std::endl;

        mesh->initialize();
        Shader* shader = nullptr;

        if(_aiScene->HasMaterials()) {
            const aiMaterial* loadedMaterial = _aiScene->mMaterials[loadedAiMesh->mMaterialIndex];
            shader = processMaterial(loadedMaterial);
        }

        _meshes->push_back(new MeshRenderer(mesh, shader));
    }

    for(size_t i = 0; i < _aiNode->mNumChildren; i++) {
        processNode(_meshes, _aiNode->mChildren[i], _aiScene);
    }
}

Mesh* ModelLoader::processMesh(const aiMesh* _aiMesh) {
    Mesh* mesh = new Mesh();

    for(size_t i = 0; i < _aiMesh->mNumVertices; i++) {
        mesh->positions.push_back({
            _aiMesh->mVertices[i].x,
            _aiMesh->mVertices[i].y,
            _aiMesh->mVertices[i].z
        });

        if(_aiMesh->HasVertexColors(0)) {
            mesh->colors.push_back({
                _aiMesh->mColors[0][i].r,
                _aiMesh->mColors[0][i].g,
                _aiMesh->mColors[0][i].b,
                _aiMesh->mColors[0][i].a
            });
        }

        if(_aiMesh->HasTextureCoords(0)) {
            mesh->uvs.push_back({
                _aiMesh->mTextureCoords[0][i].x,
                _aiMesh->mTextureCoords[0][i].y
            });
        }

        if(_aiMesh->HasNormals()) {
            mesh->normals.push_back({
                _aiMesh->mNormals[i].x,
                _aiMesh->mNormals[i].y,
                _aiMesh->mNormals[i].z
            });
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