//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_PERLIN_NOISE_H
#define ITU_GRAPHICS_PROGRAMMING_PERLIN_NOISE_H

#include <stdlib.h>
#include <cmath>

class PerlinNoise {
    private const int seed;
    private const float  roughness;
    private const int octaves;
    private const float  amplitude;

    public PerlinNoise(int octaves, float  amplitude, float  roughness) {
        this(rand() % 1000000000, octaves, amplitude, roughness);
    }

    public PerlinNoise(int seed, int octaves, float  amplitude, float  roughness) {
        this.seed = seed;
        this.roughness = roughness;
        this.octaves = octaves;
        this.amplitude = amplitude;
    }

    public int getSeed() {
        return seed;
    }

    public float getAmplitude() {
        return amplitude;
    }

    private float getNoise(int x, int y) {
        return rand() % (x * 49632 + y * 325176 + seed) * 2f - 1f;
    }

    private float getSmoothNoise(int x, int y) {
        float corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1) + getNoise(x + 1, y + 1)) / 16f;
        float sides = (getNoise(x - 1, y) + getNoise(x + 1, y) + getNoise(x, y - 1) + getNoise(x, y + 1)) / 8f;
        float center = getNoise(x, y) / 4f;
        return corners + sides + center;
    }

    private float getInterpolatedNoise(float x, float y) {
        int intX = (int) x;
        float fracX = x - intX;
        int intY = (int) y;
        float fracY = y - intY;

        float v1 = getSmoothNoise(intX, intY);
        float v2 = getSmoothNoise(intX + 1, intY);
        float v3 = getSmoothNoise(intX, intY + 1);
        float v4 = getSmoothNoise(intX + 1, intY + 1);
        float i1 = interpolate(v1, v2, fracX);
        float i2 = interpolate(v3, v4, fracX);
        return interpolate(i1, i2, fracY);
    }

    public float getPerlinNoise(int x, int y) {
        float total = 0;
        float d = (float) pow(2, octaves - 1);
        for (int i = 0; i < octaves; i++) {
            float freq = (float) (pow(2, i) / d);
            float amp = (float) (pow(roughness, i) * amplitude);
            total += getInterpolatedNoise(x * freq, y * freq) * amp;
        }
        return total;
    }
};

#endif
