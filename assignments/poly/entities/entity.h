//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_ENTITY_H
#define ITU_GRAPHICS_PROGRAMMING_ENTITY_H

#include "../engine/model.h"
#include "util/glmutils.h"

class Entity {
    private:
        Model* model;
        glm::vec3 rotation;
        float scale;

    public:
        glm::vec3 position;
        Entity(Model* model, glm::vec3 pos, glm::vec3 rot, float scale) {
            this->model = model;
            this->position = pos;
            this->rotation = rot;
            this->scale = scale;
        }

        Model* getModel() {
            return model;
        }

        glm::vec3 getPosition() {
            return position;
        }

        glm::vec3 getRotation() {
            return rotation;
        }

        float getScale() {
            return scale;
        }

        void move(glm::vec3 direction) {
            position += direction;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_ENTITY_H
