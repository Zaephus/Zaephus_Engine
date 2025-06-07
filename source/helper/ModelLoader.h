
#pragma once

#include <map>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Mesh;
class MeshRenderer;
class Shader;
class Texture2D;

struct Model;

class ModelLoader {
    public:
        enum primitiveType {
            arrow,
            cube,
            quad,
            cylinder,
            capsule,
            sphere,
            torus
        };

        static std::vector<Model*> load(primitiveType _type);
        static std::vector<Model*> load(primitiveType _type, bool _loadUnique);
        static std::vector<Model*> load(const std::string& _fileName);
        static std::vector<Model*> load(const std::string& _fileName, bool _loadUnique);

        static void dispose();

    private:
        static std::map<std::string, std::vector<Model*>> loadedModels;

        static bool isSceneValid(const aiScene* _scene, const Assimp::Importer* _importer);

        static void processNode(std::vector<Model*>* _models, const aiNode* _aiNode, const aiScene* _aiScene);
        static Mesh* processMesh(const aiMesh* _aiMesh);
        static Shader* processMaterial(const aiMaterial* _mat);

        static Texture2D* loadTexture(const aiMaterial* _mat, unsigned int _type);
};