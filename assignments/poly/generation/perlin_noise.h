//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_PERLIN_NOISE_H
#define ITU_GRAPHICS_PROGRAMMING_PERLIN_NOISE_H

#include <stdlib.h>
#include <cstdlib>
#include <cmath>

const float PI = 3.14157685;

class PerlinNoise {
private:
    int seed;
    float  roughness;
    int octaves;
    float  amplitude;

    float interpolate(float a, float b, float blend) {
        double theta = blend * PI;
        float f = (float) (1.0f - cos(glm::radians(theta))) * 0.5f;
        return a * (1.0f - f) + b * f;
    }

    float getNoise(int x, int y) {
        srand(x * 49632 + y * 325176 + seed);
        float noise = ((float)(rand()) / (float)RAND_MAX) * 2.0f - 1.0f;
        return noise;
    }

    float getSmoothNoise(int x, int y) {
        float corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1) + getNoise(x + 1, y + 1)) / 16.0f;
        float sides = (getNoise(x - 1, y) + getNoise(x + 1, y) + getNoise(x, y - 1) + getNoise(x, y + 1)) / 8.0f;
        float center = getNoise(x, y) / 4.0f;
        return corners + sides + center;
    }

    float getInterpolatedNoise(float x, float z) {
        int intX = (int) x;
        float fracX = x - intX;
        int intZ = (int) z;
        float fracZ = z - intZ;

        float v1 = getSmoothNoise(intX, intZ);
        float v2 = getSmoothNoise(intX + 1, intZ);
        float v3 = getSmoothNoise(intX, intZ + 1);
        float v4 = getSmoothNoise(intX + 1, intZ + 1);
        float i1 = interpolate(v1, v2, fracX);
        float i2 = interpolate(v3, v4, fracX);
        return interpolate(i1, i2, fracZ);
    }

public:
    PerlinNoise(int seed, int octaves, float amplitude, float  roughness) {
        if (seed == -1)
            this->seed = rand() % 1000000000;
        else
            this->seed = seed;

        this->roughness = roughness;
        this->octaves = octaves;
        this->amplitude = amplitude;
    }

    int getSeed() {
        return seed;
    }

    float getAmplitude() {
        return amplitude;
    }

    float getPerlinNoise(int x, int z) {
        float total = 0;
        float d = (float) pow(2, octaves - 1);
        for (int i = 0; i < octaves; i++) {
            float freq = (float) (pow(2, i) / d);
            float amp = (float) (pow(roughness, i) * amplitude);
            total += getInterpolatedNoise(x * freq, z * freq) * amp;
        }
        return total;
    }
};

#endif
