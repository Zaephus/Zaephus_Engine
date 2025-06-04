
#pragma once

#include <string>
#include <vector>

#include "Color.h"
#include "TextureData.h"

struct TextureData;

class Texture2D {
    public:
        std::string path;
        std::string boundUniform;

        TextureData data;

        Texture2D() = default;

        ~Texture2D() = default;

        void initialize();
        void destroy();

        void use() const;
        void setUnit(int _textureUnit);

        static Texture2D* load(const char* _name);
        static Texture2D* load(const char* _name, const TextureData& _data);
        static Texture2D* load(const std::string& _name);
        static Texture2D* load(const std::string& _name, const TextureData& _data);
        static Texture2D* load(const Color& _color);
        static Texture2D* load(const Color& _color, const TextureData& _data);

    private:
        int boundAmount = 0;

        unsigned int id = 0;
        int unit = -1;

        Color color;

        static std::vector<Texture2D*> loadedTextures;

        unsigned char* loadFromDisk(int* _width, int* _height, int* _format) const;
        unsigned char* createFromColor(int* _width, int* _height, int* _format) const;

        void bindData(const unsigned char* _imageData, int _width, int _height, int _format);

        static int checkForMatch(const std::string& _path, const Color& _color, const TextureData& _data);
};