//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_TERRAIN_H
#define ITU_GRAPHICS_PROGRAMMING_TERRAIN_H

#include "../engine/model.h"
#include "../engine/model_loader.h"
#include "heights_generator.h"
#include "../generation/colour_generator.h"

class Terrain {
    private:
        Config* config;
        float x, z;
        ColourGenerator* colourGenerator;
        Model* model;
        bool indices = true;

        float getHeight(int x, int z, HeightsGenerator* generator) {
            int max = config->vertex_count - 1;
            if (x < 0) x = 0;
            if (z < 0) z = 0;
            if (x > max) x = max;
            if (z > max) z = max;
            return generator->generateHeight(x, z, config->PERLIN);
        }

        glm::vec3 getNormal(int x, int z, HeightsGenerator* generator) {
            float HL = getHeight(x - 1, z, generator);
            float HR = getHeight(x + 1, z, generator);
            float HD = getHeight(x, z - 1, generator);
            float HU = getHeight(x, z + 1, generator);
            glm::vec3 normal = glm::vec3(HL - HR, 2.0f, HD - HU);
            glm::normalize(normal);
            return normal;
        }

        glm::vec3 getNormal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
            glm::vec3 tangentA = v1 - v0;
            glm::vec3 tangentB = v2 - v0;
            return glm::normalize(glm::cross(tangentA, tangentB));
        }

        void store(glm::vec3 pos, glm::vec3 normal, glm::vec3 colour,
                   std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
            positions.push_back(pos.x);
            positions.push_back(pos.y);
            positions.push_back(pos.z);

            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);

            colours.push_back(colour.x);
            colours.push_back(colour.y);
            colours.push_back(colour.z);
        }

        void storeTriangle(std::vector<glm::vec3> corners, std::vector<glm::vec3> cols, glm::vec3 normal, int i0, int i1, int i2,
                           std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
            store(corners.at(i0), normal, cols.at(i0), positions, normals, colours);
            store(corners.at(i1), normal, cols.at(i1), positions, normals, colours);
            store(corners.at(i2), normal, cols.at(i2), positions, normals, colours);
        }

        Model* quad(Loader* loader) {
            std::vector<float> positions = {
                    0.5f,  0.5f, 0.0f,  // top right
                    0.5f, -0.5f, 0.0f,  // bottom right
                    -0.5f, -0.5f, 0.0f,  // bottom left
                    -0.5f,  0.5f, 0.0f   // top left
            };

            std::vector<float> normals {
                0, 1, 0,
                0, 1, 0,
                0, 1, 0,
                0, 1, 0
            };

            std::vector<unsigned int> indices {
                    0, 1, 2,
                    1, 2, 3
            };

            return loader->loadToVao(positions, normals, indices);
        }

        Model* proceduralTerrain(Loader* loader) {
            HeightsGenerator* generator = new HeightsGenerator(config);

            std::vector<float> positions;
            std::vector<float> normals;
            std::vector<float> colours;
            std::vector<unsigned int> indices;

            for (int i = 0; i < config->vertex_count; i++) {
                for (int j = 0; j < config->vertex_count; j++) {
                    float height = getHeight(j, i, generator);
                    glm::vec3 position = glm::vec3(
                                    -((float) j/((float) config->vertex_count - 1)) * config->size,
                                    height,
                                    -((float) i/((float) config->vertex_count - 1)) * config->size);
                    positions.push_back(position.x);
                    positions.push_back(position.y);
                    positions.push_back(position.z);

                    glm::vec3 normal = getNormal(j, i, generator);
                    normals.push_back(normal.x);
                    normals.push_back(normal.y);
                    normals.push_back(normal.z);

                    glm::vec3 colour = glm::normalize(colourGenerator->getColour(height, config->amplitude));
                    colours.push_back(colour.x);
                    colours.push_back(colour.y);
                    colours.push_back(colour.z);
                    //std::cout << colour.x << "\t" << colour.y << "\t" << colour.z << std::endl;
                }
            }

            for(int gz = 0; gz < config->vertex_count - 1; gz++){
                for(int gx = 0; gx < config->vertex_count - 1; gx++){
                    int topLeft = (gz * config->vertex_count) + gx;
                    int topRight = topLeft + 1;
                    int bottomLeft = ((gz + 1) * config->vertex_count) + gx;
                    int bottomRight = bottomLeft + 1;
                    indices.push_back(topLeft);
                    indices.push_back(bottomLeft);
                    indices.push_back(topRight);
                    indices.push_back(topRight);
                    indices.push_back(bottomLeft);
                    indices.push_back(bottomRight);
                }
            }

            return loader->loadToVao(positions, normals, colours, indices);
        }

        Model* generateDubVertexTerrain(Loader* loader) {
            HeightsGenerator* generator = new HeightsGenerator(config);

            std::vector<float> positions;
            std::vector<float> normals;
            std::vector<float> colours;

            for (int i = 0; i < config->vertex_count; i++) {
                for (int j = 0; j < config->vertex_count; j++) {

                    std::vector<float> heights = {
                            getHeight(j, i, generator),
                            getHeight(j + 1, i, generator),
                            getHeight(j, i + 1, generator),
                            getHeight(j + 1, i + 1, generator),
                    };

                    std::vector<glm::vec3> corners = {
                            glm::vec3(-((float) j/((float) config->vertex_count - 1)) * config->size, heights.at(0), -((float) i/((float) config->vertex_count - 1)) * config->size),
                            glm::vec3(-((float) (j+1)/((float) config->vertex_count - 1)) * config->size, heights.at(1), -((float) i/((float) config->vertex_count - 1)) * config->size),
                            glm::vec3(-((float) j/((float) config->vertex_count - 1)) * config->size, heights.at(2), -((float) (i+1)/((float) config->vertex_count - 1)) * config->size),
                            glm::vec3(-((float) (j+1)/((float) config->vertex_count - 1)) * config->size, heights.at(3), -((float) (i+1)/((float) config->vertex_count - 1)) * config->size)
                    };

                    std::vector<glm::vec3> cols = {
                            glm::normalize(colourGenerator->getColour(heights.at(0), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(1), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(2), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(3), config->amplitude))
                    };

                    glm::vec3 TLNormal = getNormal(corners.at(0), corners.at(1), corners.at(2));
                    glm::vec3 BRNormal = getNormal(corners.at(2), corners.at(1), corners.at(3));

                    storeTriangle(corners, cols, TLNormal, 0, 1, 2, positions, normals, colours);
                    storeTriangle(corners, cols, BRNormal, 2, 1, 3, positions, normals, colours);
                }
            }

            indices = false;
            return loader->loadToVao(positions, normals, colours);
        }

    public:
        Terrain(int gridX, int gridZ, Loader* loader, ColourGenerator* colourGenerator, Config* config, int mode) {
            this->config = config;
            this->x = gridX * config->size;
            this->z = gridZ * config->size;
            this->colourGenerator = colourGenerator;
            switch (mode) {
                case NORMAL:
                    this->model = proceduralTerrain(loader);
                    break;
                case VERTEX_COPY:
                    this->model = generateDubVertexTerrain(loader);
                    break;
            }
        }

        float getX() {
            return x;
        }

        float getZ () {
            return z;
        }

        Model* getModel() {
            return model;
        }

        bool usesIndices() {
            return indices;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_TERRAIN_H
