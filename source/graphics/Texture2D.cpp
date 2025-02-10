
#define STB_IMAGE_IMPLEMENTATION
#include "Texture2D.h"

#include <iostream>

#include "stb_image.h"

void Texture2D::use() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::setUnit(const int textureUnit) {
    unit = textureUnit;
}

void Texture2D::destroy() {
    glDeleteTextures(1, &id);
}

void Texture2D::load(Texture2D* texture, const std::string& texturePath) {
    const std::string path = "resources/textures/" + texturePath;
    texture->path = path;

    int loadedTextureIndex = checkForMatch(texture);
    if(loadedTextureIndex >= 0) {
        *texture = loadedTextures[loadedTextureIndex];
        return;
    }

    int width, height, channelAmount;
    stbi_set_flip_vertically_on_load(texture->flipVerticallyOnLoad);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channelAmount, 0);

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->horizontalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->verticalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->magFilter);

    int format;
    switch(channelAmount) {
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: format = 0; break;
    }

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if(texture->generateMipmaps) { glGenerateMipmap(GL_TEXTURE_2D); }

        loadedTextures.emplace_back(*texture);
    }
    else {
        std::cout << "Failed to load texture!" << std::endl;
        std::cout << "Path: " << path << std::endl;
    }

    stbi_image_free(data);
}

std::vector<Texture2D> Texture2D::loadedTextures;

int Texture2D::checkForMatch(Texture2D* texture) {
    for(int i = 0; i < loadedTextures.size(); i++) {
        if(texture->path                 != loadedTextures[i].path)                 { continue; }
        if(texture->horizontalWrap       != loadedTextures[i].horizontalWrap)       { continue; }
        if(texture->verticalWrap         != loadedTextures[i].verticalWrap)         { continue; }
        if(texture->generateMipmaps      != loadedTextures[i].generateMipmaps)      { continue; }
        if(texture->minFilter            != loadedTextures[i].minFilter)            { continue; }
        if(texture->magFilter            != loadedTextures[i].magFilter)            { continue; }
        if(texture->flipVerticallyOnLoad != loadedTextures[i].flipVerticallyOnLoad) { continue; }

        return i;
    }

    return -1;
}