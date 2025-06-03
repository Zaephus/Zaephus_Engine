
#pragma once

#include <string>
#include <vector>

#include "Color.h"
#include "RenderItem.h"

class Texture2D : public RenderItem {
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

        void bind();

        void use() const;
        void setUnit(int _textureUnit);

    protected:
        void destroy() override;

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