//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_TERRAIN_H
#define ITU_GRAPHICS_PROGRAMMING_TERRAIN_H

#include "../engine/model.h"
#include "../engine/model_loader.h"
#include "heights_generator.h"
#include "../generation/colour_generator.h"
#include "../generation/mesh_generator.h"

class Terrain {
    private:
        float x, z;
        Model* model;
        bool indices = true;

    public:
        Terrain(int gridX, int gridZ, Loader* loader, ColourGenerator* colourGenerator, MeshGenerator* meshGenerator, Config* config, int mode) {
            this->x = gridX * config->terrain_size;
            this->z = gridZ * config->terrain_size;
            switch (mode) {
                case NORMAL:
                    this->model = meshGenerator->proceduralTerrain(loader);
                    break;
                case VERTEX_COPY:
                    this->model = meshGenerator->generateDubVertexTerrain(loader);
                    this->indices = false;
                    break;
                case GEOMETRY:
                    this->model = meshGenerator->generateGeoTerrain(loader);
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
