//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_TERRAIN_RENDERER_H
#define ITU_GRAPHICS_PROGRAMMING_TERRAIN_RENDERER_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>
#include <terrains/terrain.h>
#include <shaders/terrain_shader.h>
#include "model.h"

class TerrainRenderer {

    private:
        std::vector<Terrain*> terrains;

        void bind(Terrain* terrain) {
            Model* model = terrain->getModel();
            glBindVertexArray(model->getVao());
            glEnableVertexAttribArray(0); // positions
            glEnableVertexAttribArray(1); // normals
            glEnableVertexAttribArray(2); // colours
        }

        void unbind() {
            glDisableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);
        }

        void loadTerrain(Terrain* terrain, TerrainShader* shader) {
            glm::mat4 position = glm::translate(glm::vec3(terrain->getX(), 0, terrain->getZ()));
            shader->loadTransformationMatrix(glm::mat4(1.0f));
            glDrawElements(GL_TRIANGLES, terrain->getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
        }

    public:
        void prepare() {
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void render(TerrainShader* shader) {
            for (Terrain* terrain : terrains) {
                bind(terrain);
                loadTerrain(terrain, shader);
                unbind();
            }
        }

        void addTerrain(Terrain* terrain) {
            terrains.push_back(terrain);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_ENTITY_RENDERER_H
