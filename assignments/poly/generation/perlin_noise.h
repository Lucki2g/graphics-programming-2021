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
    std::vector<std::vector<float>> noiseData = {};

public:
    PerlinNoise(int seed, Config* config) {
        if (seed == -1)
            this->seed = rand() % 1000000000;
        else
            this->seed = seed;

        this->config = config;

        for (int i = 0; i < config->terrain_size + 1; i++)
            noiseData.push_back({});

        // Create and configure FastNoise object
        FastNoiseLite* noise = new FastNoiseLite(1337);
        noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise->SetFrequency(config->frequency);

        // Gather noise data
        for (int z = 0; z < config->terrain_size + 1; z++) {
            for (int x = 0; x < config->terrain_size + 1; x++) {
                noiseData.at(x).push_back(noise->GetNoise((float)x, (float)z));
            }
        }
    }

    float getPerlinNoise(int x, int z) {
        float noise = noiseData.at(x).at(z) * config->amplitude;
        return noise;
    }

    int getSeed() {
        return seed;
    }
};

#endif
