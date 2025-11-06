
#pragma once

#include <string>
#include <vector>

#include "Color.h"
#include "TextureInfo.h"

class Texture2D {
    public:
        std::string path;
        std::string boundUniform;

        TextureInfo texInfo;

        Texture2D() = default;

        void initialize();
        void destroy();

        void use() const;
        void setUnit(int _textureUnit);

        static Texture2D* load(const char* _name);
        static Texture2D* load(const char* _name, const TextureInfo& _info);
        static Texture2D* load(const std::string& _name);
        static Texture2D* load(const std::string& _name, const TextureInfo& _info);
        static Texture2D* load(const Color& _color);
        static Texture2D* load(const Color& _color, const TextureInfo& _info);

        static Texture2D* create(unsigned char* _data, int _w, int _h, int _format);

    private:
        unsigned int id = 0;
        int unit = -1;

        unsigned char* data = nullptr;
        int width = 0;
        int height = 0;
        int format = 0;

        Color color = Color::white();

        static std::vector<Texture2D*> loadedTextures;

        ~Texture2D() = default;

        void internalDestroy();

        unsigned char* loadFromDisk(int* _width, int* _height, int* _format) const;
        unsigned char* createFromColor(int* _width, int* _height, int* _format) const;

        void bindData(const unsigned char* _imageData, int _width, int _height, int _format);

        static int checkForMatch(const std::string& _path, const Color& _color, const TextureInfo& _info);
};