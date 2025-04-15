
#pragma once

#include <map>
#include <vector>

#include <assimp/material.h>

#include "Action.h"
#include "Component.h"

class aiScene;
class aiMesh;
class aiNode;

class Mesh;
class Shader;
class Texture2D;
class Light;
class Color;

struct Vector3;
struct Matrix4x4;

class MeshRenderer : public Component {
    public:
        enum primitiveType {
            cube,
            quad,
            cylinder,
            capsule,
            sphere,
            torus
        };

        static Action<void(MeshRenderer*)> modelCreatedCall;
        static Action<void(MeshRenderer*)> modelDestroyedCall;

        Shader* shader = nullptr;

        MeshRenderer() = default;
        virtual ~MeshRenderer();

        void render() const;

        void setShader(Shader* _shader);

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;

        void setColor(const std::string &name, float r, float g, float b, float a) const;
        void setColor(const std::string &name, const Color& color) const;

        void setVector3(const std::string &name, float x, float y, float z) const;
        void setVector3(const std::string &name, const Vector3& vector) const;

        void setMatrix4x4(const std::string &name, const Matrix4x4& matrix) const;
        void setTexture2D(const std::string &name, Texture2D* texture) const;

        void setLight(const std::string &name, const Light* light) const;

        bool isTransparent() const;

        static MeshRenderer* loadModel(primitiveType _type, Shader* _overrideShader = nullptr);
        static MeshRenderer* loadModel(const std::string& _fileName, Shader* _overrideShader = nullptr);

    protected:
        std::vector<Mesh*> meshes;

    private:
        std::string directory;

        static std::map<std::string, MeshRenderer*> loadedModels;
        static std::vector<Mesh*> loadedMeshes;

        static void processNode(MeshRenderer* _model, const aiNode* _aiNode, const aiScene* _aiScene);
        static void processMesh(MeshRenderer* _model, const aiMesh* _aiMesh, const aiScene* _aiScene);

        static Texture2D* loadTexture(const MeshRenderer* _model, const aiMaterial* _material, aiTextureType _type);

};