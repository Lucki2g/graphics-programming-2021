//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_HEIGHTSGENERATOR_H
#define ITU_GRAPHICS_PROGRAMMING_HEIGHTSGENERATOR_H

#include <stdlib.h>
#include <generation/height_map.h>
#include "../generation/perlin_noise.h"
#include "terrain.h"

class HeightsGenerator {

    private:
        int seed;
        PerlinNoise* noise;
        HeightMap* map;

    public:
        HeightsGenerator(Config* config) {
            this->seed = rand() % 1000000000;
            this->noise = new PerlinNoise(seed, config);
            this->map = new HeightMap("models/heightmap.png", config);
        }

        float generateHeight(int x, int z, int generator) {
            float y;
            switch (generator) {
                case 0:
                    y = map->getHeight(x, z);
                    break;
                case 1:
                    y = noise->getPerlinNoise(x, z);
                    break;
                default:
                    y = noise->getPerlinNoise(x, z);
            }
            return y;
        }
};


#endif
