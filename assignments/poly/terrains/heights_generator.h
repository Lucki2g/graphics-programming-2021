//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_HEIGHTSGENERATOR_H
#define ITU_GRAPHICS_PROGRAMMING_HEIGHTSGENERATOR_H

#include <stdlib.h>

class HeightsGenerator {

    private const float AMPLITUDE = 70.0f; // how high and low can terrain become
    private int seed;

    public HeightsGenerator() {
        this.seed = rand() % 1000000000;
    }

    public float generateHeight(int x, int y) {
        return 1;
    }
};


#endif
