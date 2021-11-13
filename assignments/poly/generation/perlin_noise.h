//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_PERLIN_NOISE_H
#define ITU_GRAPHICS_PROGRAMMING_PERLIN_NOISE_H

#include <stdlib.h>
#include <cstdlib>
#include "FastNoiseLite.h"

const float PI = 3.14157685;

class PerlinNoise {
private:
    int seed;
    Config* config;
    std::vector<float> noiseData = {};

    float interpolate(float a, float b, float blend) {
        double theta = blend * PI;
        float f = 1.0f - std::cos(glm::radians(theta)) * 0.5f;
        return a * (1.0f - f) + b * f;
    }

    float getNoise(int x, int y) {
        //srand(x * 49632 + y * 325176 + seed);
        //float noise = ((float)(rand()) / (float)RAND_MAX) * 2.0f - 1.0f;
        //return noise;
        return noiseData[x + y];
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
    PerlinNoise(int seed, Config* config) {
        if (seed == -1)
            this->seed = rand() % 1000000000;
        else
            this->seed = seed;

        this->config = config;

        // Create and configure FastNoise object
        FastNoiseLite* noise = new FastNoiseLite(seed);
        noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise->SetFrequency(config->frequency);

        // Gather noise data
        for (int y = 0; y < config->vertex_count; y++) {
            for (int x = 0; x < config->vertex_count; x++) {
                noiseData.push_back(noise->GetNoise((float)x, (float)y));
            }
        }
    }

    int getSeed() {
        return seed;
    }

    float getPerlinNoise(int x, int z) {
        float noise = noiseData.at(x + config->vertex_count * z) * config->amplitude;
        return noise;
        /*float total = 0;
        float d = std::pow(2, config->octaves - 1);
        for (int i = 0; i < config->octaves; i++) {
            float freq = std::pow(2, i) / d;
            float amp = std::pow(config->roughness, i) * config->amplitude;
            total += getInterpolatedNoise(x * freq, z * freq) * amp;
        }
        return total;*/
    }
};

#endif
