//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_RENDERER_H
#define ITU_GRAPHICS_PROGRAMMING_RENDERER_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "model.h"
#include "entities/entity.h"
#include "../shaders/static_shader.h"

class Renderer {
    public:
        void prepare() {
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void render(Entity* entity, StaticShader* shader) {
            Model* model = entity->getModel();
            glBindVertexArray(model->getVao());
            glEnableVertexAttribArray(0); // positions

            // update transformation matrix of the entity
            glm::mat4 transformation = glm::mat4(1.0f);
            transformation = glm::translate(transformation, entity->getPosition());
            transformation = glm::scale(transformation, glm::vec3(entity->getScale()));
            shader->loadTransformationMatrix(transformation);

            glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, 0);

            // clean
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_RENDERER_H
