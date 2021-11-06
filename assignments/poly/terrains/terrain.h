//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_TERRAIN_H
#define ITU_GRAPHICS_PROGRAMMING_TERRAIN_H

#include "../engine/model.h"
#include "../engine/model_loader.h"

class Terrain {
    private:
        const float SIZE = 800;
        const int VERTEX_COUNT = 128;

        float x, z;
        Model* model;
        Model* generateTerrain(Loader* loader) {
            int count = VERTEX_COUNT * VERTEX_COUNT;
            std::vector<float> vertices(count * 3);
            std::vector<float> normals(count * 3);
            std::vector<unsigned int> indices (6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

            for (int i = 0; i < VERTEX_COUNT; i++) {
                for (int j = 0; j < VERTEX_COUNT; j++) {
                    vertices.push_back(-((float) j/((float) VERTEX_COUNT - 1)) * SIZE);
                    vertices.push_back(0);
                    vertices.push_back(-((float) i/((float) VERTEX_COUNT - 1)) * SIZE);
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

            return loader->loadToVao(vertices, normals, indices);
        }

    public:
        Terrain(int gridX, int gridZ, Loader* loader) {
            this->x = gridX * SIZE;
            this->z = gridZ * SIZE;
            this->model = generateTerrain(loader);
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
