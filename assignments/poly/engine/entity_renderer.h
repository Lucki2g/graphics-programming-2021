//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_ENTITY_RENDERER_H
#define ITU_GRAPHICS_PROGRAMMING_ENTITY_RENDERER_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "model.h"
#include "entities/entity.h"
#include "../shaders/static_shader.h"

class EntityRenderer {

    private:
        std::vector<Entity*> entities;
        StaticShader* shader;

        void bind(Entity* entity) {
            Model* model = entity->getModel();
            glBindVertexArray(model->getVao());
            glEnableVertexAttribArray(0); // positions
            glEnableVertexAttribArray(1); // normals
        }

        void loadEntity(Entity* entity, StaticShader* shader) {
            // update transformation matrix of the entity
            glm::mat4 position = glm::translate(entity->getPosition());
            glm::mat4 scale = glm::scale(glm::vec3(entity->getScale()));
            glm::mat4 rotation = glm::mat4(1.0f);
            rotation = glm::rotate(rotation, glm::radians(entity->getRotation().x), glm::vec3(1,0,0));
            rotation = glm::rotate(rotation, glm::radians(entity->getRotation().y), glm::vec3(0,1,0));
            rotation = glm::rotate(rotation, glm::radians(entity->getRotation().z), glm::vec3(0,0,1));
            shader->loadTransformationMatrix(position * rotation * scale);

            glDrawElements(GL_TRIANGLES, entity->getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
        }

        void unbind() {
            // clean
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindVertexArray(0);
        }

    public:
        EntityRenderer(glm::mat4 projectionMatrix) {
            shader = new StaticShader();
            shader->Shader::start();
            shader->loadProjectionMatrix(projectionMatrix);
            shader->Shader::stop();
        }

        void render(Light* sun, glm::mat4 viewMatrix) {
            shader->Shader::start();
            shader->loadLight(sun);
            shader->loadViewMatrix(viewMatrix);
            for (Entity* entity : entities) {
                bind(entity);
                loadEntity(entity, shader);
                unbind();
            }
            shader->Shader::stop();
        }

        void addEntity(Entity* entity) {
            entities.push_back(entity);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_ENTITY_RENDERER_H
