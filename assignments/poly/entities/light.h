//
// Created by Lucki on 06-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_LIGHT_H
#define ITU_GRAPHICS_PROGRAMMING_LIGHT_H

#include <glm/vec3.hpp>

class Light {
    private:
    glm::vec3 position;
        glm::vec3 colour;
        float intensity;

    public:
        Light(glm::vec3 pos, glm::vec3 col) {
            this->position = pos;
            this->colour = col;
            this->intensity = 1.0f;
        }

        Light(glm::vec3 pos, glm::vec3 col, float intensity) {
            this->position = pos;
            this->colour = col;
            this->intensity = intensity;
        }

        glm::vec3 getColour() {
            return colour;
        }

        glm::vec3 getPosition() {
            return position;
        }

        float getIntensity() {
            return intensity;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_LIGHT_H
