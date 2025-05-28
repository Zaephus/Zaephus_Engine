
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

        explicit Texture2D(const std::string& _name);

        void initialize();

        void use() const;
        void setUnit(int _textureUnit);
        void destroy() const;

    private:
        unsigned int id = 0;
        int unit = -1;

        static std::vector<Texture2D> loadedTextures;

        static int checkForMatch(const Texture2D* _texture);
};