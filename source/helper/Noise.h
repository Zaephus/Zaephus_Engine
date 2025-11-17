
#pragma once
#include "Vectors/Vector2.h"
#include "Vectors/Vector2Int.h"

class Texture2D;

/**
 * A class containing functions for generating noise, like Perlin or Voronoi noise.
 */
class Noise {
    public:
        /**
         * Generates a Perlin noise texture. This is a deterministic, pseudo-random function, it will always return the same value with the same inputs.
         * @param _size The size of the texture.
         * @param _offset The offset for the calculation.
         * @param _cellSize The size of the cells used to calculate the noise.
         * @param _octaves Amount of different octaves.
         * @param _persistence Visibility of every octave compared to the previous octave. A value of 0.5 means every octave is half as visible as the previous.
         * @return A Texture2D containing the calculated Perlin noise.
         */
        static Texture2D* perlinTexture(const Vector2Int& _size, const Vector2& _offset, float _cellSize, unsigned int _octaves, float _persistence);

        /**
         * Generates a Voronoi noise texture. This is a fully random algorithm, it always returns a different texture, even with the same parameters.
         * @param _size The size of the texture.
         * @param _offset The offset for the calculation.
         * @param _cellSize The size of the cells used to calculate the noise.
         * @param _octaves Amount of different octaves.
         * @param _persistence Visibility of every octave compared to the previous octave. A value of 0.5 means every octave is half as visible as the previous.
         * @return A Texture2D containing the calculated Voronoi noise.
         */
        static Texture2D* voronoiTexture(const Vector2Int& _size, const Vector2& _offset, float _cellSize, unsigned int _octaves, float _persistence);

        /**
         * Generates a white noise texture. Every pixel in this texture is completely randomized.
         * @param _size The size of the texture.
         * @return A Texture2D containing the calculated white noise.
         */
        static Texture2D* whiteNoiseTexture(const Vector2Int& _size);

        /**
         * Generates Perlin noise per pixel. This is a deterministic, pseudo-random function, it will always return the same value with the same inputs.
         * @param _x Pixel coordinate on the X-axis.
         * @param _y Pixel coordinate on the Y-axis.
         * @param _z Pixel coordinate on the Z-axis.
         * @param _octaves Amount of different octaves.
         * @param _persistence Visibility of every octave compared to the previous octave. A value of 0.5 means every octave is half as visible as the previous.
         * @return Noise value for the given coordinates
         */
        static float perlin(float _x, float _y, float _z, unsigned int _octaves, float _persistence);

    private:
        static int perm[];

        static float calcPerlin(float _x, float _y, float _z);

        static float fade(float _t);
        static float grad(int _hash, float _x, float _y, float _z);
};