
#include "TextureInfo.h"

bool TextureInfo::operator==(const TextureInfo& _info) const {
    return horizontalWrap       == _info.horizontalWrap
        && verticalWrap         == _info.verticalWrap
        && generateMipmaps      == _info.generateMipmaps
        && minFilter            == _info.minFilter
        && magFilter            == _info.magFilter
        && flipVerticallyOnLoad == _info.flipVerticallyOnLoad;
}

bool TextureInfo::operator!=(const TextureInfo& _info) const {
    return !(*this == _info);
}