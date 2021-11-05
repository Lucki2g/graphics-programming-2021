//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_ENTITY_H
#define ITU_GRAPHICS_PROGRAMMING_ENTITY_H

#include "../engine/model.h"
#include "../glmutils.h"

class Entity {
    private:
        Model* model;
        glm::vec3 position;
        float scale;

    public:
        Entity(Model* model, glm::vec3 pos, float scale) {
            this->model = model;
            this->position = pos;
            this->scale = scale;
        }

        Model* getModel() {
            return model;
        }

        glm::vec3 getPosition() {
            return position;
        }

        float getScale() {
            return scale;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_ENTITY_H
