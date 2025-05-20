
#pragma once

#include <map>
#include <string>
#include <vector>

class ShaderUniformItem;
class Color;
class Texture2D;
class Light;

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

        explicit Shader(const char* _fragmentPath);
        Shader(const char* _vertexPath, const char* _fragmentPath);
        ~Shader();

        void initialize();
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

        void setLight(const std::string& _name, const Light* _light);

        [[nodiscard]] bool isTransparent() const;

        static Shader* unlitShader(float _r, float _g, float _b, float _a);
        static Shader* unlitShader(const Color& _c);

        static Shader* diffuseShader(float _r, float _g, float _b, float _a);
        static Shader* diffuseShader(float _r, float _g, float _b, float _a, float _shininess);
        static Shader* diffuseShader(const Color& _c);
        static Shader* diffuseShader(const Color& _c, float _shininess);

        static Shader* instancedDiffuseShader(float _r, float _g, float _b, float _a);
        static Shader* instancedDiffuseShader(float _r, float _g, float _b, float _a, float _shininess);
        static Shader* instancedDiffuseShader(const Color& _c);
        static Shader* instancedDiffuseShader(const Color& _c, float _shininess);

        static Shader* textureShader(const std::string& _diffusePath, const std::string& _specularPath, float _shininess);
        static Shader* textureShader(Texture2D* _diffuse, Texture2D* _specular, float _shininess);

    private:
        unsigned int id = 0;

        const char* vertexPath = "";
        const char* fragmentPath = "";

        std::map<std::string, Color> assignedColors;
        std::vector<Texture2D*> boundTextures;

        std::map<std::string, ShaderUniformItem> uniformQueue;

        static Shader* activeShader;

        static std::string load(const std::string& _fileName);

        static unsigned int compile(const std::string& _code, unsigned int _shaderType);
        static unsigned int createProgram(const unsigned int& _vertexShader, const unsigned int& _fragmentShader);
};