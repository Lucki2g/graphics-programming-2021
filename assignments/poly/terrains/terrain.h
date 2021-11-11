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
        const float SIZE = 800;
        const int VERTEX_COUNT = 128;

        float x, z;
        ColourGenerator* colourGenerator;
        Model* model;

        float getHeight(int x, int z, HeightsGenerator* generator) {
            return generator->generateHeight(x, z);
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
            HeightsGenerator* generator = new HeightsGenerator();

            std::vector<float> positions;
            std::vector<float> normals;
            std::vector<float> colours;
            std::vector<unsigned int> indices;

            for (int i = 0; i < VERTEX_COUNT; i++) {
                for (int j = 0; j < VERTEX_COUNT; j++) {
                    float height = getHeight(j, i, generator);
                    positions.push_back(-((float) j/((float) VERTEX_COUNT - 1)) * SIZE);
                    positions.push_back(height);
                    positions.push_back(-((float) i/((float) VERTEX_COUNT - 1)) * SIZE);

                    glm::vec3 normal = getNormal(j, i, generator);
                    normals.push_back(normal.x);
                    normals.push_back(normal.y);
                    normals.push_back(normal.z);

                    glm::vec3 colour = glm::normalize(colourGenerator->getColour(height, generator->getAmplitude()));
                    colours.push_back(colour.x);
                    colours.push_back(colour.y);
                    colours.push_back(colour.z);
                    //std::cout << colour.x << "\t" << colour.y << "\t" << colour.z << std::endl;
                }
            }

            for(int gz = 0; gz < VERTEX_COUNT - 1; gz++){
                for(int gx = 0; gx < VERTEX_COUNT - 1; gx++){
                    int topLeft = (gz * VERTEX_COUNT) + gx;
                    int topRight = topLeft + 1;
                    int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
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

        Model* generateTerrain(Loader* loader) {
            std::vector<float> positions;
            std::vector<float> normals;
            std::vector<unsigned int> indices;

            for (int i = 0; i < VERTEX_COUNT; i++) {
                for (int j = 0; j < VERTEX_COUNT; j++) {
                    positions.push_back(-((float) j/((float) VERTEX_COUNT - 1)) * SIZE);
                    positions.push_back(0);
                    positions.push_back(-((float) i/((float) VERTEX_COUNT - 1)) * SIZE);
                    normals.push_back(0);
                    normals.push_back(1);
                    normals.push_back(0);
                }
            }

            for(int gz = 0; gz < VERTEX_COUNT - 1; gz++){
                for(int gx = 0; gx < VERTEX_COUNT - 1; gx++){
                    int topLeft = (gz * VERTEX_COUNT) + gx;
                    int topRight = topLeft + 1;
                    int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
                    int bottomRight = bottomLeft + 1;
                    indices.push_back(topLeft);
                    indices.push_back(bottomLeft);
                    indices.push_back(topRight);
                    indices.push_back(topRight);
                    indices.push_back(bottomLeft);
                    indices.push_back(bottomRight);
                }
            }

            return loader->loadToVao(positions, normals, indices);
        }

    public:
        Terrain(int gridX, int gridZ, Loader* loader, ColourGenerator* colourGenerator) {
            this->x = gridX * SIZE;
            this->z = gridZ * SIZE;
            this->colourGenerator = colourGenerator;
            // this->model = generateTerrain(loader);
            // this->model = generateTerrain(loader);
            this->model = proceduralTerrain(loader);
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
};

#endif //ITU_GRAPHICS_PROGRAMMING_TERRAIN_H
