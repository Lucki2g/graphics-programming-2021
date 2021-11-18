//
// Created by Lucki on 18-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_MASTER_RENDERER_H
#define ITU_GRAPHICS_PROGRAMMING_MASTER_RENDERER_H

#include "entity_renderer.h"
#include "terrain_renderer.h"
#include "water_renderer.h"

class MasterRenderer {

    private:
        EntityRenderer* entities;
        TerrainRenderer* terrains;

public:
        void init(glm::mat4 projectionMatrix) {
            entities = new EntityRenderer(projectionMatrix);
            terrains = new TerrainRenderer(projectionMatrix);
        }

        void prepare() {
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void render(Light* sun, glm::mat4 viewMatrix, Config* config, glm::vec4 clipPlane) {
            prepare();
            entities->render(sun, viewMatrix);
            terrains->render(sun, clipPlane, viewMatrix, config);
        }

        void addEntity(Entity* ent) {
            entities->addEntity(ent);
        }

        void addTerrain(Terrain* terrain, int mode) {
            terrains->addTerrain(terrain, mode);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_MASTER_RENDERER_H
