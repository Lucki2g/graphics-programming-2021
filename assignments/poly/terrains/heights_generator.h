//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_HEIGHTSGENERATOR_H
#define ITU_GRAPHICS_PROGRAMMING_HEIGHTSGENERATOR_H

#include <stdlib.h>
#include "../generation/perlin_noise.h"

class HeightsGenerator {

    private:
        const float AMPLITUDE = 70.0f; // how high and low can terrain become
        int seed;
        PerlinNoise* noise;

    public:
        HeightsGenerator() {
            this->seed = rand() % 1000000000;
            this->noise = new PerlinNoise(seed, 3, AMPLITUDE, 0.35f);
        }

        float generateHeight(int x, int z) {
            float y = noise->getPerlinNoise(x, z);
            return y;
        }
};


#endif
