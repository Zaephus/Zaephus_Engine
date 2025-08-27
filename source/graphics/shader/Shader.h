
#pragma once

#include <map>
#include <string>
#include <vector>

#include "ShaderUniformItem.h"

class DirectionalLight;
class Texture2D;
class PointLight;

struct Color;
struct Vector3;
struct Matrix4x4;

class Shader {
    public:
        enum renderOrder {
            opaques,
            transparents
        };
        renderOrder order = opaques;

        bool depthTestEnabled = true;
        bool drawAsWireframe = false;

        explicit Shader(const std::string& _fragmentPath);
        Shader(const std::string& _vertexPath, const std::string& _fragmentPath);

        void initialize();
        void destroy();

        void bind();

        void applyUniforms();

        void setBool(const std::string& _name, bool _value);
        void setInt(const std::string& _name, int _value);
        void setFloat(const std::string& _name, float _value);

        void setColor(const std::string& _name, float _r, float _g, float _b, float _a);
        void setColor(const std::string& _name, const Color& _value);

        [[nodiscard]] Color getColor(const std::string& _name) const;

        void setVector3(const std::string& _name, float _x, float _y, float _z);
        void setVector3(const std::string& _name, const Vector3& _value);

        void setMatrix4x4(const std::string& _name, const Matrix4x4& _value);
        void setTexture2D(const std::string& _name, Texture2D* _texture);

        void setDirLight(const std::string& _name, const DirectionalLight* _light);
        void setPointLight(const std::string& _name, const PointLight* _light);

        [[nodiscard]] bool isTransparent() const;

        static Shader* unlitShader(float _r, float _g, float _b, float _a);
        static Shader* unlitShader(const Color& _c);

        static Shader* unlitTextureShader(const std::string& _texturePath, float _uvScale);
        static Shader* unlitTextureShader(Texture2D* _texture, float _uvScale);

        static Shader* diffuseShader(float _r, float _g, float _b, float _a);
        static Shader* diffuseShader(float _r, float _g, float _b, float _a, float _shininess);
        static Shader* diffuseShader(const Color& _c);
        static Shader* diffuseShader(const Color& _c, float _shininess);

        static Shader* diffuseTextureShader(const std::string& _diffusePath, const std::string& _specularPath, float _uvScale, float _shininess);
        static Shader* diffuseTextureShader(Texture2D* _diffuse, Texture2D* _specular, float _uvScale, float _shininess);

        static Shader* instancedUnlitShader(float _r, float _g, float _b, float _a);
        static Shader* instancedUnlitShader(const Color& _c);

        static Shader* instancedUnlitTextureShader(const std::string& _texturePath, float _uvScale);
        static Shader* instancedUnlitTextureShader(Texture2D* _texture, float _uvScale);

        static Shader* instancedDiffuseShader(float _r, float _g, float _b, float _a);
        static Shader* instancedDiffuseShader(float _r, float _g, float _b, float _a, float _shininess);
        static Shader* instancedDiffuseShader(const Color& _c);
        static Shader* instancedDiffuseShader(const Color& _c, float _shininess);

        static Shader* instancedDiffuseTextureShader(const std::string& _diffusePath, const std::string& _specularPath, float _uvScale, float _shininess);
        static Shader* instancedDiffuseTextureShader(Texture2D* _diffuse, Texture2D* _specular, float _uvScale, float _shininess);

    private:
        unsigned int id = 0;

        std::string vertexPath;
        std::string fragmentPath;

        std::map<std::string, Color> assignedColors;
        std::vector<Texture2D*> boundTextures;

        std::map<std::string, ShaderUniformItem> uniformQueue;

        static const std::string shaderVersion;

        static Shader* activeShader;

        ~Shader() = default;

        void internalDestroy();

        static std::string processVertexCode(const std::string& _vertexPath);
        static std::string processFragmentCode(const std::string& _fragmentPath);

        static std::string load(const std::string& _fileName);
        static std::vector<std::string> convertToLines(const std::string& _fileText);

        unsigned int compile(const std::string& _code, unsigned int _shaderType);
        unsigned int createProgram(const unsigned int& _vertexShader, const unsigned int& _fragmentShader);
};