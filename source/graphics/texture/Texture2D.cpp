
#define STB_IMAGE_IMPLEMENTATION

#include "Texture2D.h"

#include <iostream>

#include <stb_image.h>
#include <glad/gl.h>

#include "Action.h"
#include "Renderer.h"

void Texture2D::initialize() {
    if(id != 0) { return; }

    int width, height, format;
    unsigned char* data = nullptr;

    if(!path.empty()) { data = loadFromDisk(&width, &height, & format); }
    else { data = createFromColor(&width, &height, & format); }

    bindData(data, width, height, format);

    if(!path.empty()) { stbi_image_free(data); }

    Renderer::cleanupRenderObjectsCall.bind<Texture2D, &Texture2D::internalDestroy>(this);
}

void Texture2D::destroy() {
    Renderer::destroyRenderObjectCall.bind<Texture2D, &Texture2D::internalDestroy>(this);
}

void Texture2D::internalDestroy() {
    Renderer::destroyRenderObjectCall.unbind<Texture2D, &Texture2D::internalDestroy>(this);
    Renderer::cleanupRenderObjectsCall.unbind<Texture2D, &Texture2D::internalDestroy>(this);

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
    int channelAmount;

    stbi_set_flip_vertically_on_load(data.flipVerticallyOnLoad);
    unsigned char* imageData = stbi_load(path.c_str(), _width, _height, &channelAmount, 0);

    switch(channelAmount) {
        case 2: *_format = GL_RG; break;
        case 3: *_format = GL_RGB; break;
        case 4: *_format = GL_RGBA; break;
        default: *_format = 0; break;
    }

    return imageData;
}

unsigned char* Texture2D::createFromColor(int* _width, int* _height, int* _format) const {
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

void Texture2D::bindData(const unsigned char* _imageData, const int _width, const int _height, const int _format) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, data.horizontalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, data.verticalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, data.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, data.magFilter);
    if(_imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, _format, GL_UNSIGNED_BYTE, _imageData);
        if(data.generateMipmaps) { glGenerateMipmap(GL_TEXTURE_2D); }
    }
    else {
        std::cerr << "Failed to load texture!" << std::endl;
        std::cerr << "Path: " << path << std::endl;
    }
}

Texture2D* Texture2D::load(const char* _name) { return load(std::string(_name), TextureData()); }
Texture2D* Texture2D::load(const char* _name, const TextureData& _data) { return load(std::string(_name), _data); }
Texture2D* Texture2D::load(const std::string& _name) { return load(_name, TextureData()); }
Texture2D* Texture2D::load(const std::string& _name, const TextureData& _data) {
    const std::string path = "resources/textures/" + _name;

    const int loadedTextureIndex = checkForMatch(path, Color::white(), _data);
    if(loadedTextureIndex >= 0) {
        return loadedTextures[loadedTextureIndex];
    }

    Texture2D* tex = new Texture2D;
    tex->path = path;
    tex->data = _data;

    loadedTextures.push_back(tex);

    return tex;
}

Texture2D* Texture2D::load(const Color& _color) { return load(_color, TextureData()); }
Texture2D* Texture2D::load(const Color& _color, const TextureData& _data) {
    const int loadedTextureIndex = checkForMatch("", _color, _data);
    if(loadedTextureIndex >= 0) {
        return loadedTextures[loadedTextureIndex];
    }

    Texture2D* tex = new Texture2D;
    tex->color = _color;
    tex->data = _data;

    loadedTextures.push_back(tex);

    return tex;
}

std::vector<Texture2D*> Texture2D::loadedTextures;

int Texture2D::checkForMatch(const std::string& _path, const Color& _color, const TextureData& _data) {
    for(int i = 0; i < loadedTextures.size(); i++) {
        if(_path  != loadedTextures[i]->path)  { continue; }
        if(_color != loadedTextures[i]->color) { continue; }
        if(_data  != loadedTextures[i]->data)  { continue; }

        return i;
    }

    return -1;
}