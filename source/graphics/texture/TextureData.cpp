
#include "TextureData.h"

bool TextureData::operator==(const TextureData& _data) const {
    return horizontalWrap       == _data.horizontalWrap
        && verticalWrap         == _data.verticalWrap
        && generateMipmaps      == _data.generateMipmaps
        && minFilter            == _data.minFilter
        && magFilter            == _data.magFilter
        && flipVerticallyOnLoad == _data.flipVerticallyOnLoad;
}

bool TextureData::operator!=(const TextureData& _data) const {
    return !(*this == _data);
}