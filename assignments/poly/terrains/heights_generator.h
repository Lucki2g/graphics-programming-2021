//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_HEIGHTSGENERATOR_H
#define ITU_GRAPHICS_PROGRAMMING_HEIGHTSGENERATOR_H

#include <stdlib.h>
#include "../generation/perlin_noise.h"

class HeightsGenerator {

    private:
        int seed;
        PerlinNoise* noise;

    public:
        HeightsGenerator(Config* config) {
            this->seed = rand() % 1000000000;
            this->noise = new PerlinNoise(seed, config);
        }

        float generateHeight(int x, int z) {
            float y = noise->getPerlinNoise(x, z);
            return y;
        }
};


#endif
