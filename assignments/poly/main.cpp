//
// Created by Lucki on 05-11-2021.
//

#include <util/guis/im_gui.h>
#include "engine/window_manager.h"
#include "engine/model_loader.h"
#include "engine/terrain_renderer.h"
#include "engine/entity_renderer.h"
#include "util/primitives.h"
#include "util/obj_reader.h"
#include <map>
#include <engine/water_renderer.h>
#include <engine/gui_renderer.h>
#include <engine/master_renderer.h>
#include <chrono>

void print(std::string s);
void renderTerrain(Config* config, NormalTerrainShader* normalShader, Light* sun, WindowManager* windowManager,
                   TerrainRenderer* terrainRenderer, FlatTerrainShader* flatShader, CopyTerrainShader* copyShader,
                   GeometryTerrainShader* geoShader, glm::vec4 clippingPlane);

int main () {

    /******************* SETUP *****************/
    Config* config = new Config();
    WindowManager* windowManager = new WindowManager(config);
    Loader* loader = new Loader();
    ObjReader* objloader = new ObjReader();
    MasterRenderer* renderer = new MasterRenderer();
    ColourGenerator* colourGenerator = new ColourGenerator(config);
    MeshGenerator* meshGenerator = new MeshGenerator(config, colourGenerator);

    /******************* WINDOW *****************/
    windowManager->createWindow();
    renderer->init(windowManager->getProjectionMatrix());
    WaterRenderer* waterRenderer = new WaterRenderer(windowManager->getProjectionMatrix(), config);

    /******************* MODELS & TERRAIN *****************/

    Model* cloud1 = objloader->loadObjModel("models/cloud1.obj", loader);
    Entity* ent_cloud1 = new Entity(cloud1, glm::vec3(2, 14, 10), glm::vec3(), 0.015f);
    renderer->addEntity(ent_cloud1);
    Entity* ent_cloud1_2 = new Entity(cloud1, glm::vec3(4, 12, 32), glm::vec3(), 0.025f);
    renderer->addEntity(ent_cloud1_2);

    Model* cloud2 = objloader->loadObjModel("models/cloud2.obj", loader);
    Entity* ent_cloud2 = new Entity(cloud2, glm::vec3(12, 18, 7), glm::vec3(), 0.015f);
    renderer->addEntity(ent_cloud2);

    Model* cloud3 = objloader->loadObjModel("models/cloud3.obj", loader);
    Entity* ent_cloud3 = new Entity(cloud3, glm::vec3(24, 10, 1), glm::vec3(), 0.01f);
    renderer->addEntity(ent_cloud3);

    // std::cout << config->lightPosition.x << "," << config->lightPosition.y << "," << config->lightPosition.z << std::endl;
    Light* sun = new Light(config->lightPosition, config->lightColour, config->lightIntensity);

    Terrain* terrain1 = new Terrain(0, 0, loader, colourGenerator, meshGenerator, config, NORMAL);
    renderer->addTerrain(terrain1, NORMAL);
    renderer->addTerrain(terrain1, MESH);
    renderer->addTerrain(terrain1, FLAT);

    Terrain* terrain2 = new Terrain(0, 0, loader, colourGenerator, meshGenerator, config, VERTEX_COPY);
    renderer->addTerrain(terrain2, VERTEX_COPY);

    Terrain* terrain3 = new Terrain(0, 0, loader, colourGenerator, meshGenerator, config, GEOMETRY);
    renderer->addTerrain(terrain3, GEOMETRY);

    Terrain* terrain4 = new Terrain(0, 0, loader, colourGenerator, meshGenerator, config, FLAT_S);
    renderer->addTerrain(terrain4, FLAT_S);

    Terrain* terrain5 = new Terrain(0, 0, loader, colourGenerator, meshGenerator, config, EQUILINOX);
    renderer->addTerrain(terrain5, EQUILINOX);

    Gui* gui = new Gui(config, sun, renderer);

    /******************* WATER *****************/
    WaterFBOs* waterFbOs = new WaterFBOs(config);

    Model* waterModel = meshGenerator->generateWater(loader);
    Entity* waterEntity = new Entity(waterModel, glm::vec3(0, config->water_height, 0), glm::vec3(), 1);
    waterRenderer->setWater(waterEntity, waterFbOs);

    GuiRenderer* guiRenderer = new GuiRenderer(loader);
    std::vector<GuiTexture*> guis = {};
    guis.push_back(new GuiTexture(waterFbOs->getReflectionTex(), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f, 0.25f)));
    guis.push_back(new GuiTexture(waterFbOs->getRefractionTex(), glm::vec2(-0.5f, 0.5f), glm::vec2(0.25f, -0.25f)));
    guis.push_back(new GuiTexture(waterFbOs->getRefractionDepthTex(), glm::vec2(-0.5f, -0.5f), glm::vec2(0.25f, -0.25f), true));

    /******************* LOOP *****************/
    float FPS = 60;
    float lastTime = 0.0f;

    /******************* LOOP *****************/
    while (!windowManager->shouldClose()) {
        // update current time
        float time = glfwGetTime();
        float d = time - lastTime;
        if (d >= 1 / FPS) {
            lastTime = time;

            glEnable(GL_CLIP_DISTANCE0); // enable clipping plane for water texture

            // water
            // render scene to reflection texture
            waterFbOs->bindReflection();
            float d = 2 * (camera->getPosition().y - waterEntity->getPosition().y);
            camera->move(glm::vec3(0, -d, 0)); // move camera under the water
            camera->invertPitch();
            camera->calculateForward();

            renderer->render(sun, windowManager->getViewMatrix(), config, glm::vec4(0, 1, 0, -waterEntity->getPosition().y + 0.1f));
            camera->move(glm::vec3(0, d, 0)); // move camera back
            camera->invertPitch();
            camera->calculateForward();
            waterFbOs->unbindFrameBuffer();

            // render scene to refraction texture
            waterFbOs->bindRefraction();
            renderer->render(sun, windowManager->getViewMatrix(), config, glm::vec4(0, -1, 0, waterEntity->getPosition().y + 2.0f));
            waterFbOs->unbindFrameBuffer();

            // render
            renderer->render(sun, windowManager->getViewMatrix(), config, glm::vec4(0, 1, 0, config->amplitude + 20.0f));
            waterRenderer->render(sun, windowManager->getViewMatrix(), camera->getPosition(), config);

            // gui
            if (config->showFbos) guiRenderer->render(guis);
            if (windowManager->shouldDrawGui())
                gui->drawGui(loader, colourGenerator, meshGenerator, config);


            // process input
            windowManager->processInput();
            //update
            windowManager->updateWindow();
        }
    }

    /******************* CLEAN *****************/
    waterFbOs->clean();
    gui->clean();
    loader->clean();
    windowManager->closeWindow();
}


void print(std::string s) {
    std::cout << s << std::endl;
}
