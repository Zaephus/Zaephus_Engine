
#define STB_IMAGE_IMPLEMENTATION

#include "Texture2D.h"

#include <iostream>

#include <stb_image.h>
#include <glad/gl.h>

Texture2D::Texture2D(const char* _name)
    : Texture2D(std::string(_name)) {}
Texture2D::Texture2D(const std::string& _name) {
    horizontalWrap = GL_REPEAT;
    verticalWrap = GL_REPEAT;

    generateMipmaps = true;

    minFilter = GL_LINEAR_MIPMAP_LINEAR;
    magFilter = GL_LINEAR;

    flipVerticallyOnLoad = false;

    boundUniform = "";
    path = "resources/textures/" + _name;
}

Texture2D::Texture2D(const Color& _color) {
    horizontalWrap = GL_REPEAT;
    verticalWrap = GL_REPEAT;

    generateMipmaps = false;

    minFilter = GL_NEAREST;
    magFilter = GL_NEAREST;

    flipVerticallyOnLoad = false;

    boundUniform = "";
    path = "";

    color = _color;
}

void Texture2D::initialize() {
    boundAmount++;

    const int loadedTextureIndex = checkForMatch(this);
    if(loadedTextureIndex >= 0) {
        *this = loadedTextures[loadedTextureIndex];
        return;
    }

    int width, height, format;
    unsigned char* data = nullptr;

    if(path != "") { data = loadFromDisk(&width, &height, & format); }
    else { data = createFromColor(&width, &height, & format); }

    bindData(data, width, height, format);

    if(path != "") { stbi_image_free(data); }
}

void Texture2D::destroy() {
    boundAmount--;

    if(boundAmount > 0) { return; }

    glDeleteTextures(1, &id);

    delete this;
}

void Texture2D::use() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::setUnit(const int _textureUnit) {
    unit = _textureUnit;
}

unsigned char* Texture2D::loadFromDisk(int* _width, int* _height, int* _format) const {
    std::cout << "Loaded a texture from disk: " << path << std::endl;

    int channelAmount;

    stbi_set_flip_vertically_on_load(flipVerticallyOnLoad);
    unsigned char* data = stbi_load(path.c_str(), _width, _height, &channelAmount, 0);

    switch(channelAmount) {
        case 2: *_format = GL_RG; break;
        case 3: *_format = GL_RGB; break;
        case 4: *_format = GL_RGBA; break;
        default: *_format = 0; break;
    }

    return data;
}

unsigned char* Texture2D::createFromColor(int* _width, int* _height, int* _format) const {
    std::cout << "Created a texture from a color: " << color.toString() << std::endl;

    *_width = 1;
    *_height = 1;
    *_format = GL_RGBA;

    unsigned char* data = new unsigned char[4];
    data[0] = static_cast<unsigned char>(color.r * 255);
    data[1] = static_cast<unsigned char>(color.g * 255);
    data[2] = static_cast<unsigned char>(color.b * 255);
    data[3] = static_cast<unsigned char>(color.a * 255);

    return &data[0];
}

void Texture2D::bindData(const unsigned char* _data, const int _width, const int _height, const int _format) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizontalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, verticalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    if(_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, _format, GL_UNSIGNED_BYTE, _data);
        if(generateMipmaps) { glGenerateMipmap(GL_TEXTURE_2D); }

        loadedTextures.push_back(*this);
    }
    else {
        std::cerr << "Failed to load texture!" << std::endl;
        std::cerr << "Path: " << path << std::endl;
    }
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
        if(_texture->color                != loadedTextures[i].color)                { continue; }

        return i;
    }

    return -1;
}