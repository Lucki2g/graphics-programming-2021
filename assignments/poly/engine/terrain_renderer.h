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
#include <map>
#include "model.h"

class TerrainRenderer {

    private:
        std::map<int, std::vector<Terrain*>> terrains;

        void bind(Terrain* terrain) {
            Model* model = terrain->getModel();
            glBindVertexArray(model->getVao());
            glEnableVertexAttribArray(0); // positions
            glEnableVertexAttribArray(1); // normals
            glEnableVertexAttribArray(2); // colours
        }

        void unbind() {
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glBindVertexArray(0);
        }

        void loadTerrain(Terrain* terrain, TerrainShader* shader) {
            glm::mat4 position = glm::translate(glm::vec3(terrain->getX(), 0, terrain->getZ()));
            shader->loadTransformationMatrix(position);
            std::cout << terrain->getModel()->getVertexCount() << std::endl;
            if (terrain->usesIndices())
                glDrawElements(GL_TRIANGLES, terrain->getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
            else
                glDrawArrays(GL_TRIANGLES, 0, terrain->getModel()->getVertexCount());
        }

    public:
        void prepare() {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void render(TerrainShader* shader, int mode) {
            for (Terrain* terrain : terrains[mode]) {
                bind(terrain);
                loadTerrain(terrain, shader);
                unbind();
            }
        }

        void addTerrain(Terrain* terrain, int type) {
            terrains[type].push_back(terrain);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_ENTITY_RENDERER_H
