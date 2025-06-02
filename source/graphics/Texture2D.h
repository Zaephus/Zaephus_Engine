
#pragma once

#include <string>
#include <vector>

#include "Color.h"

class Texture2D {

    public:

        int horizontalWrap;
        int verticalWrap;

        bool generateMipmaps;

        int minFilter;
        int magFilter;

        bool flipVerticallyOnLoad;

        std::string boundUniform;
        std::string path;

        explicit Texture2D(const char* _name);
        explicit Texture2D(const std::string& _name);
        explicit Texture2D(const Color& _color);

        void initialize();

        void use() const;
        void setUnit(int _textureUnit);
        void destroy() const;

    private:
        unsigned int id = 0;
        int unit = -1;

        Color color;

        static std::vector<Texture2D> loadedTextures;

        unsigned char* loadFromDisk(int* _width, int* _height, int* _format) const;
        unsigned char* createFromColor(int* _width, int* _height, int* _format) const;

        void bindData(const unsigned char* _data, int _width, int _height, int _format);

        static int checkForMatch(const Texture2D* _texture);
};