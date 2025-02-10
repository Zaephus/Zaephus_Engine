
#pragma once

#include "glad/gl.h"

#include <map>
#include <string>
#include <vector>

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

        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();

        void use() const;

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;

        void setColor(const std::string& name, float r, float g, float b, float a);
        void setColor(const std::string& name, const Color& color);

        Color getColor(const std::string& name) const;

        void setVector3(const std::string& name, float x, float y, float z) const;
        void setVector3(const std::string& name, const Vector3& vector) const;

        void setMatrix4x4(const std::string& name, const Matrix4x4& matrix) const;
        void setTexture2D(const std::string& name, Texture2D* texture);

        void setLight(const std::string& name, const Light* light) const;

        static Shader* unlitShader(float r, float g, float b, float a);
        static Shader* unlitShader(const Color& c);

        static Shader* diffuseShader(float r, float g, float b, float a);
        static Shader* diffuseShader(float r, float g, float b, float a, float shininess);
        static Shader* diffuseShader(const Color& c);
        static Shader* diffuseShader(const Color& c, float shininess);

        static Shader* textureShader(const std::string& diffusePath, const std::string& specularPath, float shininess);
        static Shader* textureShader(Texture2D* diffuse, Texture2D* specular, float shininess);

    private:
        unsigned int id;
        std::map<std::string, Color> assignedColors;
         std::vector<Texture2D*> boundTextures;

        static std::string load(const std::string& _fileName);
        static unsigned int compile(const std::string& code, GLenum shaderType);
        static unsigned int createProgram(const unsigned int& vertexShader, const unsigned int& fragmentShader);
};