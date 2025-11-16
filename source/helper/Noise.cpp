
#include "Noise.h"

#include <cmath>
#include <iostream>

#include "Random.h"
#include "Texture2D.h"
#include "ZMath.h"

int Noise::perm[] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
    36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
    234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134,
    139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
    220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1,
    216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116,
    188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124,
    123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16,
    58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
    70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110,
    79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193,
    238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
    49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45,
    127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141,
    128, 195, 78, 66, 215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131, 13,
    201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37,
    240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252,
    219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125,
    136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158,
    231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245,
    40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187,
    208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198,
    173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255,
    82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183,
    170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
    172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112,
    104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
    241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106,
    157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93,
    222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

Texture2D* Noise::perlinTexture(const int _w, const int _h, const float _xOffset, const float _yOffset, const float _cellSize, const unsigned int _octaves, const float _persistence) {
    float* texels = new float[_w * _h * 3];

    int i = 0;

    for(int x = 0; x < _w; x++) {
        for(int y = 0; y < _h; y++) {
            const float xVal = static_cast<float>(x);
            const float yVal = static_cast<float>(y);

            const float val = perlin(
                (_xOffset + xVal) / _cellSize,
                (_yOffset + yVal) / _cellSize,
                0,
                _octaves,
                _persistence
            );

            texels[i]   = val;
            texels[i+1] = val;
            texels[i+2] = val;

            i += 3;
        }
    }

    return Texture2D::create(&texels[0], _w, _h, GL_RGB);
}

Texture2D* Noise::voronoiTexture(const Vector2Int& _size, const Vector2& _offset, const Vector2Int& _cellAmount, const unsigned int _octaves, const float _persistence) {
    float* texels = new float[_size.x * _size.y * 3] {};

    unsigned int octaves;
    if(_octaves <= 0) {
        octaves = 1;
        std::cerr << "ERROR: Using an octaves setting of zero." << std::endl;
    }
    else {
        octaves = _octaves;
    }

    float freq = 1.0f;
    float amp = 1.0f;
    float maxVal = 0.0f;

    for(int octave = 0; octave < octaves; octave++) {
        Vector2Int cellAmount = {
            _cellAmount.x * static_cast<int>(freq) + 1,
            _cellAmount.y * static_cast<int>(freq) + 1
        };

        const float cellWidth = static_cast<float>(_size.x) / static_cast<float>(cellAmount.y - 1);
        const float cellHeight = static_cast<float>(_size.y) / static_cast<float>(cellAmount.x - 1);

        Vector2 points[cellAmount.x][cellAmount.y];

        for(int x = 0; x < cellAmount.x; x++) {
            for(int y = 0; y < cellAmount.y; y++) {
                points[x][y] = {
                    (static_cast<float>(x) - 0.5f) * cellWidth + Random::range(0.0f, cellWidth),
                    (static_cast<float>(y) - 0.5f) * cellHeight + Random::range(0.0f, cellHeight)
                };
            }
        }

        int i = 0;

        // Pixel Calc
        for(int x = 0; x < _size.x; x++) {
            for(int y = 0; y < _size.y; y++) {
                Vector2 uv = {
                    static_cast<float>(x) / cellWidth - 0.5f,
                    static_cast<float>(y) / cellHeight - 0.5f
                };

                // Cell number
                Vector2Int grid(
                    static_cast<int>(std::floor(uv.x)),
                    static_cast<int>(std::floor(uv.y))
                );

                float pointDistance = 10000.0f;

                for(int nx = -1; nx <= 1; nx++) {
                    for(int ny = -1; ny <= 1; ny++) {
                        Vector2Int num = {
                            grid.x + nx,
                            grid.y + ny
                        };

                        if(num.x < 0 || num.x >= cellAmount.x || num.y < 0 || num.y >= cellAmount.y) {
                            continue;
                        }

                        Vector2 point = {
                            points[num.x][num.y].x / cellWidth,
                            points[num.x][num.y].y / cellHeight
                        };

                        float dist = Vector2::distance(uv, point);
                        pointDistance = std::min(dist, pointDistance);
                    }
                }

                float distVal = ZMath::smoothStep(1.7f - pointDistance, 0.2f, 2.0f);
                distVal = 1 - distVal;

                Vector3 col(distVal, distVal, distVal);

                col *= amp;

                // Apply colors to pixels
                texels[i]   += col.x;
                texels[i+1] += col.y;
                texels[i+2] += col.z;

                i += 3;
            }
        }

        maxVal  += amp;

        amp *= _persistence;
        freq *= 2.0f;
    }

    for(int i = 0; i < _size.x * _size.y * 3; i++) {
        texels[i] /= maxVal;
    }

    return Texture2D::create(&texels[0], _size.x, _size.y, GL_RGB);
}


Texture2D* Noise::whiteNoiseTexture(const int _w, const int _h) {
    float* texels = new float[_w * _h * 3];

    int i = 0;

    for(int x = 0; x < _w; x++) {
        for(int y = 0; y < _h; y++) {
            const float val = Random::range(0.0f, 1.0f);

            texels[i] = val;
            texels[i+1] = val;
            texels[i+2] = val;

            i += 3;
        }
    }

    return Texture2D::create(&texels[0], _w, _h, GL_RGB);
}

float Noise::perlin(const float _x, const float _y, const float _z, const unsigned int _octaves, const float _persistence) {
    unsigned int octaves;
    if(_octaves <= 0) {
        octaves = 1;

        std::cerr << "ERROR: Using an octaves setting of zero." << std::endl;
    }
    else {
        octaves = _octaves;
    }

    float total = 0;
    float freq = 1;
    float amp = 1;
    float maxVal = 0;

    for(int i = 0; i < octaves; i++) {
        total += calcPerlin(_x * freq, _y * freq, _z * freq) * amp;

        maxVal  += amp;

        amp *= _persistence;
        freq *= 2;
    }

    return total / maxVal;
}


float Noise::calcPerlin(const float _x, const float _y, const float _z) {
    const int xi = static_cast<int>(std::floor(_x)) & 255;
    const int yi = static_cast<int>(std::floor(_y)) & 255;
    const int zi = static_cast<int>(std::floor(_z)) & 255;

    const float xf = _x - std::floor(_x);
    const float yf = _y - std::floor(_y);
    const float zf = _z - std::floor(_z);

    const float u = fade(xf);
    const float v = fade(yf);
    const float w = fade(zf);

    const int aaa = perm[perm[perm[xi  ] + yi  ] + zi  ];
    const int aba = perm[perm[perm[xi  ] + yi+1] + zi  ];
    const int aab = perm[perm[perm[xi  ] + yi  ] + zi+1];
    const int abb = perm[perm[perm[xi  ] + yi+1] + zi+1];
    const int baa = perm[perm[perm[xi+1] + yi  ] + zi  ];
    const int bba = perm[perm[perm[xi+1] + yi+1] + zi  ];
    const int bab = perm[perm[perm[xi+1] + yi  ] + zi+1];
    const int bbb = perm[perm[perm[xi+1] + yi+1] + zi+1];

    float x1 = ZMath::lerp(grad(aaa, xf, yf, zf), grad(baa, xf-1, yf, zf), u);
    float x2 = ZMath::lerp(grad(aba, xf, yf-1, zf), grad(bba, xf-1, yf-1, zf), u);

    const float y1 = ZMath::lerp(x1, x2, v);

    x1 = ZMath::lerp(grad(aab, xf, yf, zf-1), grad(bab, xf-1, yf, zf-1), u);
    x2 = ZMath::lerp(grad(abb, xf, yf-1, zf-1), grad(bbb, xf-1, yf-1, zf-1), u);

    const float y2 = ZMath::lerp(x1, x2, v);

    return (ZMath::lerp(y1, y2, w) + 1) / 2;
}

float Noise::fade(const float _t) {
    return _t * _t * _t * (_t * (_t * 6 - 15) + 10);
}

float Noise::grad(const int _hash, const float _x, const float _y, const float _z) {
    switch(_hash & 0xF) {
        case 0x0: return  _x + _y;
        case 0x1: return -_x + _y;
        case 0x2: return  _x - _y;
        case 0x3: return -_x - _y;
        case 0x4: return  _x + _z;
        case 0x5: return -_x + _z;
        case 0x6: return  _x - _z;
        case 0x7: return -_x - _z;
        case 0x8: return  _y + _z;
        case 0x9: return -_y + _z;
        case 0xA: return  _y - _z;
        case 0xB: return -_y - _z;
        case 0xC: return  _y + _x;
        case 0xD: return -_y + _z;
        case 0xE: return  _y - _x;
        case 0xF: return -_y - _z;
        default: return 0;
    }
}