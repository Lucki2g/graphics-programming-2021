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

    float getPerlinNoise(int x, int z) {
        float noise = noiseData.at(x + config->vertex_count * z) * config->amplitude;
        return noise;
    }

    int getSeed() {
        return seed;
    }
};

#endif
