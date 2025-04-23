
#define STB_IMAGE_IMPLEMENTATION
#include "Texture2D.h"

#include <iostream>

#include "stb_image.h"

#include "glad/gl.h"

Texture2D::Texture2D() {
    horizontalWrap = GL_REPEAT;
    verticalWrap = GL_REPEAT;

    generateMipmaps = true;

    minFilter = GL_LINEAR_MIPMAP_LINEAR;
    magFilter = GL_LINEAR;

    flipVerticallyOnLoad = false;
}

void Texture2D::use() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::setUnit(const int _textureUnit) {
    unit = _textureUnit;
}

void Texture2D::destroy() const {
    glDeleteTextures(1, &id);
}

void Texture2D::load(Texture2D* _texture, const std::string& _texturePath) {
    const std::string path = "resources/textures/" + _texturePath;
    _texture->path = path;

    const int loadedTextureIndex = checkForMatch(_texture);
    if(loadedTextureIndex >= 0) {
        *_texture = loadedTextures[loadedTextureIndex];
        return;
    }

    int width, height, channelAmount;
    stbi_set_flip_vertically_on_load(_texture->flipVerticallyOnLoad);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channelAmount, 0);

    glGenTextures(1, &_texture->id);
    glBindTexture(GL_TEXTURE_2D, _texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _texture->horizontalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _texture->verticalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _texture->minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _texture->magFilter);

    int format;
    switch(channelAmount) {
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: format = 0; break;
    }

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if(_texture->generateMipmaps) { glGenerateMipmap(GL_TEXTURE_2D); }

        loadedTextures.emplace_back(*_texture);
    }
    else {
        std::cout << "Failed to load texture!" << std::endl;
        std::cout << "Path: " << path << std::endl;
    }

    stbi_image_free(data);
}

std::vector<Texture2D> Texture2D::loadedTextures;

int Texture2D::checkForMatch(const Texture2D* _texture) {
    for(int i = 0; i < loadedTextures.size(); i++) {
        if(_texture->path                 != loadedTextures[i].path)                 { continue; }
        if(_texture->horizontalWrap       != loadedTextures[i].horizontalWrap)       { continue; }
        if(_texture->verticalWrap         != loadedTextures[i].verticalWrap)         { continue; }
        if(_texture->generateMipmaps      != loadedTextures[i].generateMipmaps)      { continue; }
        if(_texture->minFilter            != loadedTextures[i].minFilter)            { continue; }
        if(_texture->magFilter            != loadedTextures[i].magFilter)            { continue; }
        if(_texture->flipVerticallyOnLoad != loadedTextures[i].flipVerticallyOnLoad) { continue; }

        return i;
    }

    return -1;
}