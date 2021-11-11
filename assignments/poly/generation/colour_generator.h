//
// Created by Lucki on 08-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_COLOUR_GENERATOR_H
#define ITU_GRAPHICS_PROGRAMMING_COLOUR_GENERATOR_H

#include <vector>
#include "../util/glmutils.h"
class ColourGenerator {
    const std::vector<glm::vec3> PALLET = {
            glm::vec3(201, 178, 99),
            glm::vec3(135, 184, 82),
            glm::vec3(80, 171, 93),
            glm::vec3(120, 120, 120),
            glm::vec3(200, 200, 210)
    };

    const float SPREAD = 0.45f,
            HALF_SPREAD = SPREAD / 2.0f,
            PART = 1.0f / (PALLET.size() - 1);

    float clamp(float n, float lower, float upper) {
        return std::max(lower, std::min(n, upper));
    }

    public:
        glm::vec3 getColour(float height, float amplitude) {
            float v = (height + amplitude) / (amplitude * 2);
            v = clamp((v - HALF_SPREAD) * (1.0f / SPREAD), 0.0f, 0.9999f);
            int fst = std::floor(v / PART);
            float blend = (v - (fst * PART)) / PART;
            glm::vec3 c1 = PALLET[fst];
            glm::vec3 c2 = PALLET[fst + 1];
            return c1;
            //return glm::mix(c1, c2, blend);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_COLOUR_GENERATOR_H
