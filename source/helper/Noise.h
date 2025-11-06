
#pragma once

class Texture2D;

class Noise {
    public:
        static Texture2D* perlinTexture(int _w, int _h, float _xOffset, float _yOffset, float _size, unsigned int _octaves, float _persistence);
        static float perlin(float _x, float _y, float _z, unsigned int _octaves, float _persistence);

    private:
        static int perm[];

        static float calcPerlin(float _x, float _y, float _z);

        static float fade(float _t);
        static float grad(int _hash, float _x, float _y, float _z);
        static float lerp(float _a, float _b, float _t);
};