
#pragma once

#include <vector>
#include <string>
#include <map>

class aiMaterial;
class aiMesh;
class aiNode;
class aiScene;
namespace Assimp { class Importer; }

class Mesh;
class MeshRenderer;
class Shader;
class Texture2D;

class ModelLoader {
    public:
        enum primitiveType {
            cube,
            quad,
            cylinder,
            capsule,
            sphere,
            torus
        };

        static std::vector<MeshRenderer*> load(primitiveType _type);
        static std::vector<MeshRenderer*> load(const std::string& _fileName);

    private:
        static std::map<std::string, std::vector<MeshRenderer*>> loadedModels;

        static bool isSceneValid(const aiScene* _scene, const Assimp::Importer* _importer);

        static void processNode(std::vector<MeshRenderer*>* _meshes, const aiNode* _aiNode, const aiScene* _aiScene);
        static Mesh* processMesh(const aiMesh* _aiMesh);
        static Shader* processMaterial(const aiMaterial* _aiMaterial);

        // TODO: Add texture support for loading materials.
        // static Texture2D* loadTexture(const Mesh* _model, const aiMaterial* _material, unsigned int _type);
};