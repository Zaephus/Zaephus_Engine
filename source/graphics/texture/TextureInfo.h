
#pragma once

#include <string>

#include <glad/gl.h>

struct TextureInfo {
    int horizontalWrap = GL_REPEAT;
    int verticalWrap = GL_REPEAT;

    bool generateMipmaps = true;

    int minFilter = GL_LINEAR_MIPMAP_LINEAR;
    int magFilter = GL_LINEAR;

    bool flipVerticallyOnLoad = false;

    bool operator==(const TextureInfo& _info) const;
    bool operator!=(const TextureInfo& _info) const;
};