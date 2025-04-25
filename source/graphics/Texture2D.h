
#pragma once

#include <string>
#include <vector>

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

        Texture2D();

        void use() const;
        void setUnit(int _textureUnit);
        void destroy() const;

        static void load(Texture2D* _texture, const std::string& _texturePath);

    private:
        unsigned int id = -1;
        int unit = -1;

        static std::vector<Texture2D> loadedTextures;

        static int checkForMatch(const Texture2D* _texture);
};