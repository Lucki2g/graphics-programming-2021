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
#include <shaders/normal/normal_terrain_shader.h>
#include <shaders/vertexcopy/copy_terrain_shader.h>
#include <shaders/geometry/geomoetry_terrain_shader.h>
#include <shaders/flat/flat_terrain_shader.h>
#include "model.h"

class TerrainRenderer {

    private:
        std::map<int, std::vector<Terrain*>> terrains;
        std::map<int, std::unique_ptr<TerrainShader>> shaders;

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
            // std::cout << terrain->getModel()->getVertexCount() << std::endl;
            if (terrain->usesIndices())
                glDrawElements(GL_TRIANGLES, terrain->getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
            else
                glDrawArrays(GL_TRIANGLES, 0, terrain->getModel()->getVertexCount());
        }

    public:
        TerrainRenderer(glm::mat4 projectionMatrix) {
            TerrainShader* normalShader;
            normalShader = new NormalTerrainShader();
            normalShader->Shader::start();
            normalShader->loadProjectionMatrix(projectionMatrix);
            normalShader->Shader::stop();
            shaders.insert(std::pair<int, std::unique_ptr<TerrainShader>>(NORMAL, normalShader));
            shaders.insert(std::pair<int, std::unique_ptr<TerrainShader>>(MESH, normalShader));

            TerrainShader* copyShader;
            copyShader = new CopyTerrainShader();
            copyShader->Shader::start();
            copyShader->loadProjectionMatrix(projectionMatrix);
            copyShader->Shader::stop();
            shaders.insert(std::pair<int, std::unique_ptr<TerrainShader>>(VERTEX_COPY, copyShader));

            TerrainShader* geoShader;
            geoShader = new GeometryTerrainShader();
            geoShader->Shader::start();
            geoShader->loadProjectionMatrix(projectionMatrix);
            geoShader->Shader::stop();
            shaders.insert(std::pair<int, std::unique_ptr<TerrainShader>>(GEOMETRY, geoShader));

            TerrainShader* flatShader;
            flatShader = new FlatTerrainShader();
            flatShader->Shader::start();
            flatShader->loadProjectionMatrix(projectionMatrix);
            flatShader->Shader::stop();
            shaders.insert(std::pair<int, std::unique_ptr<TerrainShader>>(FLAT, flatShader));
        }

        void render(Light* sun, glm::vec4 clipPlane, glm::mat4 viewMatrix, Config* config) {

            int mode = config->generationSetting;
            TerrainShader* shader = shaders[mode].get();

            shader->Shader::start();
            shader->loadLight(sun);
            shader->loadWaterClippingPlane(clipPlane);
            shader->loadViewMatrix(viewMatrix);
            shader->loadAmbientLighting(config->ambientLightColour, config->ambientLightIntensity, config->ambientReflectance);
            shader->loadDiffuseLighting(config->diffuseReflectance);
            for (Terrain* terrain : terrains[mode]) {
                bind(terrain);
                loadTerrain(terrain, shader);
                unbind();
            }
            shader->Shader::stop();
        }

        void addTerrain(Terrain* terrain, int type) {
            terrains[type].push_back(terrain);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_ENTITY_RENDERER_H
