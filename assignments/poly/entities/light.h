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

    public:
        Light(glm::vec3 pos, glm::vec3 col) {
            this->position = pos;
            this->colour = col;
        }

        glm::vec3 getColour() {
            return colour;
        }

        glm::vec3 getPosition() {
            return position;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_LIGHT_H
