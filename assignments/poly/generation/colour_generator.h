//
// Created by Lucki on 08-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_COLOUR_GENERATOR_H
#define ITU_GRAPHICS_PROGRAMMING_COLOUR_GENERATOR_H

#include <vector>
#include "../util/glmutils.h"
class ColourGenerator {

    private:
        Config* config;

        float clamp(float n, float lower, float upper) {
            this->config = config;
            return std::max(lower, std::min(n, upper));
        }

    public:
        ColourGenerator(Config* config) {
            this->config = config;
        }

        glm::vec3 getColour(float height, float amplitude) {
            float v = (height + amplitude) / (amplitude * 2);
            v = clamp((v - config->half_spread) * (1.0f / config->spread), 0.0f, 0.9999f);
            int fst = std::floor(v / config->part);
            float blend = (v - (fst * config->part)) / config->part;
            glm::vec3 c1 = config->pallet[fst];
            glm::vec3 c2 = config->pallet[fst + 1];
            return c1;
            //return glm::mix(c1, c2, blend);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_COLOUR_GENERATOR_H
