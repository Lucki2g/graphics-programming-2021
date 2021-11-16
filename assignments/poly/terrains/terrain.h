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

            float** heights = generateHeights(generator);
            glm::vec3** colours = generateColours(heights);
            glm::vec3** normals = generateNormals(heights);

            std::vector<float> out_positions;
            std::vector<float> out_normals;
            std::vector<float> out_colours;
            std::vector<unsigned int> out_indices;

            for (int z = 0; z < config->terrain_size; z++) {
                for (int x = 0; x < config->terrain_size; x++) {
                    float height = heights[z][x];
                    /*glm::vec3 position = glm::vec3(  -((float) x/((float) config->terrain_size - 1)) * config->size,
                                                     height,
                                                     -((float) z/((float) config->terrain_size - 1)) * config->size
                    );*/

                    glm::vec3 position = glm::vec3(z, height, x);
                    out_positions.push_back(position.x);
                    out_positions.push_back(position.y);
                    out_positions.push_back(position.z);

                    glm::vec3 normal = normals[z][x];
                    out_normals.push_back(normal.x);
                    out_normals.push_back(normal.y);
                    out_normals.push_back(normal.z);

                    glm::vec3 colour = glm::normalize(colours[z][x]);
                    out_colours.push_back(colour.x);
                    out_colours.push_back(colour.y);
                    out_colours.push_back(colour.z);
                    //std::cout << colour.x << "\t" << colour.y << "\t" << colour.z << std::endl;
                }
            }

            generateIndices(out_indices);
            /*for(int col = 0; col < config->terrain_size - 1; col++){
                for(int row = 0; row < config->terrain_size - 1; row++){
                    int topLeft = (row * config->terrain_size) + col;
                    int topRight = topLeft + 1;
                    int bottomLeft = ((row + 1) * config->terrain_size) + col;
                    int bottomRight = bottomLeft + 1;
                    if (row % 2 == 0) {
                        indices.push_back(topLeft);
                        indices.push_back(bottomLeft);
                        indices.push_back(col % 2 == 0 ? topRight : bottomRight);
                        indices.push_back(bottomRight);
                        indices.push_back(topRight);
                        indices.push_back(col % 2 == 0 ? bottomLeft : topLeft);
                    }else{
                        indices.push_back(topRight);
                        indices.push_back(topLeft);
                        indices.push_back(col % 2 == 0 ? bottomRight : bottomLeft);
                        indices.push_back(bottomLeft);
                        indices.push_back(bottomRight);
                        indices.push_back(col % 2 == 0 ? topLeft : topRight);
                    }
                }
            }*/

            return loader->loadToVao(out_positions, out_normals, out_colours, out_indices);
        }

        Model* generateDubVertexTerrain(Loader* loader) {
            HeightsGenerator* generator = new HeightsGenerator(config);

            float** gen_heights = generateHeights(generator);

            std::vector<float> out_positions;
            std::vector<float> out_normals;
            std::vector<float> out_colours;

            for (int z = 0; z < config->terrain_size; z++) {
                for (int x = 0; x < config->terrain_size; x++) {

                    std::vector<float> heights = {
                            getHeight(x, z, gen_heights),
                            getHeight(x + 1, z, gen_heights),
                            getHeight(x, z + 1, gen_heights),
                            getHeight(x + 1, z + 1, gen_heights),
                    };

                    /*std::vector<glm::vec3> corners = {
                            glm::vec3(-((float) x / ((float) config->vertex_count - 1)) * config->terrain_size, heights.at(0), -((float) z / ((float) config->vertex_count - 1)) * config->size),
                            glm::vec3(-((float) (x + 1) / ((float) config->vertex_count - 1)) * config->terrain_size, heights.at(1), -((float) z / ((float) config->vertex_count - 1)) * config->size),
                            glm::vec3(-((float) x / ((float) config->vertex_count - 1)) * config->terrain_size, heights.at(2), -((float) (z + 1) / ((float) config->vertex_count - 1)) * config->size),
                            glm::vec3(-((float) (x + 1) / ((float) config->vertex_count - 1)) * config->terrain_size, heights.at(3), -((float) (z + 1) / ((float) config->vertex_count - 1)) * config->size)
                    };*/

                    std::vector<glm::vec3> corners = {
                            glm::vec3(x, heights.at(0), z),
                            glm::vec3(x + 1, heights.at(1), z),
                            glm::vec3(x, heights.at(2), z + 1),
                            glm::vec3(x + 1, heights.at(3), z + 1)
                    };

                    std::vector<glm::vec3> cols = {
                            glm::normalize(colourGenerator->getColour(heights.at(0), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(1), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(2), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(3), config->amplitude))
                    };

                    glm::vec3 TLNormal = getNormal(corners.at(0), corners.at(1), corners.at(2));
                    glm::vec3 BRNormal = getNormal(corners.at(2), corners.at(1), corners.at(3));

                    storeTriangle(corners, cols, TLNormal, 0, 1, 2, out_positions, out_normals, out_colours);
                    storeTriangle(corners, cols, BRNormal, 2, 1, 3, out_positions, out_normals, out_colours);
                }
            }

            indices = false;
            return loader->loadToVao(out_positions, out_normals, out_colours);
        }

        Model* generateGeoTerrain(Loader* loader) {
            HeightsGenerator* generator = new HeightsGenerator(config);

            float** heights = generateHeights(generator);
            glm::vec3** colours = generateColours(heights);

            std::vector<float> out_positions;
            std::vector<float> out_colours;
            std::vector<unsigned int> out_indices;

            for (int z = 0; z < config->terrain_size; z++) {
                for (int x = 0; x < config->terrain_size; x++) {

                    float height = heights[z][x];
                    glm::vec3 position = glm::vec3(z, height, x);
                    out_positions.push_back(position.x);
                    out_positions.push_back(position.y);
                    out_positions.push_back(position.z);

                    glm::vec3 colour = glm::normalize(colours[z][x]);
                    out_colours.push_back(colour.x);
                    out_colours.push_back(colour.y);
                    out_colours.push_back(colour.z);
                }
            }

            generateIndices(out_indices);

            return loader->loadToVaoNoNormals(out_positions, out_colours, out_indices);
        }
        /************** INDICES ***************/
        void generateIndices(std::vector<unsigned int> &indices) {
            int size = config->terrain_size;
            //std::cout << "INDICES: " << std::endl;
            for (int col = 0; col < size - 1; col++) {
                for (int row = 0; row < size - 1; row++) {
                    unsigned int topLeft = (row * size) + col;
                    unsigned int topRight = topLeft + 1;
                    unsigned int bottomLeft = ((row + 1) * size) + col;
                    unsigned int bottomRight = bottomLeft + 1;
                    indices.push_back(topLeft);
                    indices.push_back(bottomLeft);
                    indices.push_back(bottomRight);
                    //std::cout << "T1: " << topLeft << " " << bottomLeft << " " << bottomRight << std::endl;
                    indices.push_back(topLeft);
                    indices.push_back(bottomRight);
                    indices.push_back(topRight);
                    //std::cout << "T2: " << topLeft << " " << bottomRight << " " << topRight << std::endl;
                }
            }
        }

        /************** HEIGHT ***************/
        float** generateHeights(HeightsGenerator* generator) {
            float** heights = new float*[config->terrain_size + 1];
            std::cout << "HEIGHTS: " << std::endl;
            for (int z = 0; z < config->terrain_size + 1; z++) {
                heights[z] = new float[config->terrain_size + 1];
                for (int x = 0; x < config->terrain_size + 1; x++) {
                    float height = generator->generateHeight(x, z, config->PERLIN);
                    //std::cout << "x: " << x << " z: " << z << " = " << height << std::endl;
                    heights[z][x] = height;
                }
            }
            return heights;
        }

        /************** COLOUR ***************/
        glm::vec3** generateColours(float** heights) {
            glm::vec3** colours = new glm::vec3* [config->terrain_size + 1];
            //std::cout << "COLOURS: " << std::endl;
            for (int z = 0; z < config->terrain_size + 1; z++) {
                colours[z] = new glm::vec3 [config->terrain_size + 1];
                for (int x = 0; x < config->terrain_size + 1; x++) {
                    float height = heights[z][x];
                    glm::vec3 colour = colourGenerator->getColour(height, config->amplitude);
                    //std::cout << "height:" << height << " = " << colour.x << "," << colour.y << "," << colour.z << std::endl;
                    colours[z][x] = colour;
                }
            }
            return colours;
        }

        /************** NORMAL ***************/
        glm::vec3** generateNormals(float** heights) {
            glm::vec3** normals = new glm::vec3* [config->terrain_size + 1];
            //std::cout << "NORMALS: " << std::endl;
            for (int z = 0; z < config->terrain_size + 1; z++) {
                normals[z] = new glm::vec3 [config->terrain_size + 1];
                for (int x = 0; x < config->terrain_size + 1; x++) {
                    glm::vec3 normal = getNormal(x, z, heights);
                    //std::cout << " = " << normal.x << "," << normal.y << "," << normal.z << std::endl;
                    normals[z][x] = normal;
                }
            }
            return normals;
        }

        glm::vec3 getNormal(int x, int z, float** heights) {
            float HL = getHeight(x - 1, z, heights);
            float HR = getHeight(x + 1, z, heights);
            float HD = getHeight(x, z - 1, heights);
            float HU = getHeight(x, z + 1, heights);
            glm::vec3 normal = glm::vec3(HL - HR, 2.0f, HD - HU);
            glm::normalize(normal);
            return normal;
        }

        float getHeight(int x, int z, float** heights) {
            int max = config->terrain_size - 1;
            x = std::min(std::max(0, x), max);
            z = std::min(std::max(0, z), max);
            return heights[z][x];
        }

        glm::vec3 getNormal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
            glm::vec3 tangentA = v1 - v0;
            glm::vec3 tangentB = v2 - v0;
            return glm::normalize(glm::cross(tangentA, tangentB));
        }

    public:
        Terrain(int gridX, int gridZ, Loader* loader, ColourGenerator* colourGenerator, Config* config, int mode) {
            this->config = config;
            this->x = gridX * config->terrain_size;
            this->z = gridZ * config->terrain_size;
            this->colourGenerator = colourGenerator;
            switch (mode) {
                case NORMAL:
                    this->model = proceduralTerrain(loader);
                    break;
                case VERTEX_COPY:
                    this->model = generateDubVertexTerrain(loader);
                    break;
                case GEOMETRY:
                    this->model = generateGeoTerrain(loader);
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
