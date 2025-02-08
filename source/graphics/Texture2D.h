
#pragma once

#include <string>
#include <vector>

#include "glad/gl.h"

class Texture2D {

    public:

        int horizontalWrap = GL_REPEAT;
        int verticalWrap = GL_REPEAT;

        bool generateMipmaps = true;

        int minFilter = GL_LINEAR_MIPMAP_LINEAR;
        int magFilter = GL_LINEAR;

        bool flipVerticallyOnLoad = false;

        std::string boundUniform;
        std::string path;

        Texture2D() = default;

        void use() const;
        void setUnit(int textureUnit);
        void destroy();

        static void load(Texture2D* texture, const std::string& texturePath);

    private:
        unsigned int id = -1;
        int unit = -1;

        static std::vector<Texture2D> loadedTextures;

        static int checkForMatch(Texture2D* texture);
};