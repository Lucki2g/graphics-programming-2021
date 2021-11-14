//
// Created by Lucki on 05-11-2021.
//

#include <util/gui.h>
#include "engine/window_manager.h"
#include "engine/model_loader.h"
#include "engine/terrain_renderer.h"
#include "engine/entity_renderer.h"
#include "util/primitives.h"
#include "shaders/static_shader.h"
#include "shaders/terrain_shader.h"
#include "shaders/shader_program.h"
#include "util/obj_reader.h"
#include "shaders/normal/normal_terrain_shader.h"
#include <map>
#include <shaders/vertexcopy/copy_terrain_shader.h>

void print(std::string s);

int main () {
    /******************* SETUP *****************/
    Config* config = new Config();
    WindowManager* windowManager = new WindowManager(config);
    Loader* loader = new Loader();
    ObjReader* objloader = new ObjReader();
    EntityRenderer* entityRenderer = new EntityRenderer();
    TerrainRenderer* terrainRenderer = new TerrainRenderer();
    Gui* gui = new Gui(config);
    ColourGenerator* colourGenerator = new ColourGenerator(config);

    /******************* WINDOW *****************/
    windowManager->createWindow();

    /******************* SHADERS *****************/
    StaticShader* staticShader = new StaticShader();
    staticShader->Shader::start();
    staticShader->loadProjectionMatrix(windowManager->getProjectionMatrix());
    staticShader->Shader::stop();

    NormalTerrainShader* normalShader = new NormalTerrainShader();
    normalShader->Shader::start();
    normalShader->loadProjectionMatrix(windowManager->getProjectionMatrix());
    normalShader->loadAmbientLighting(config->ambientLightColour, config->ambientLightIntensity, config->ambientReflectance);
    normalShader->loadDiffuseLighting(config->diffuseReflectance);
    normalShader->Shader::stop();

    CopyTerrainShader* copyShader = new CopyTerrainShader();
    copyShader->Shader::start();
    copyShader->loadProjectionMatrix(windowManager->getProjectionMatrix());
    copyShader->loadAmbientLighting(config->ambientLightColour, config->ambientLightIntensity, config->ambientReflectance);
    copyShader->loadDiffuseLighting(config->diffuseReflectance);
    copyShader->Shader::stop();

    /******************* MODELS & TERRAIN *****************/
    /*Model* model = objloader->loadObjModel("models/dragon.obj", loader);
    Entity* entity = new Entity(model, glm::vec3(0, 0, -2), glm::vec3(), 1);
    entityRenderer->addEntity(entity);*/

    Model* cubeModel = loader->loadToVao(cubeVertices, cubeColors, cubeNormals, cubeIndices);
    Entity* cubeEntity = new Entity(cubeModel, config->lightPosition, glm::vec3(), 1);
    entityRenderer->addEntity(cubeEntity);

    Light* sun = new Light(config->lightPosition, config->lightColour, config->lightIntensity);

    Terrain* terrain1 = new Terrain(0, 0, loader, colourGenerator, config, NORMAL);
    terrainRenderer->addTerrain(terrain1, NORMAL);

    Terrain* terrain2 = new Terrain(0, 0, loader, colourGenerator, config, VERTEX_COPY);
    terrainRenderer->addTerrain(terrain2, VERTEX_COPY);

    /******************* LOOP *****************/
    while (!windowManager->shouldClose()) {
        // prepare
        terrainRenderer->prepare();
        // process input
        windowManager->processInput();
        // start shader
        // entities
        staticShader->Shader::start();
        staticShader->loadLight(sun);
        staticShader->loadViewMatrix(windowManager->getViewMatrix());
        entityRenderer->render(staticShader);
        staticShader->Shader::stop();
        // terrain
        switch (config->generationSetting) {
            case NORMAL: case MESH:
                normalShader->TerrainShader::Shader::start();
                normalShader->TerrainShader::loadLight(sun);
                normalShader->TerrainShader::loadViewMatrix(windowManager->getViewMatrix());
                terrainRenderer->render(normalShader, config->generationSetting);
                normalShader->TerrainShader::Shader::stop();
                break;
            case VERTEX_COPY:
                copyShader->Shader::start();
                copyShader->loadLight(sun);
                copyShader->loadViewMatrix(windowManager->getViewMatrix());
                terrainRenderer->render(copyShader, config->generationSetting);
                copyShader->Shader::stop();
                break;
        }

        // gui
        if (windowManager->shouldDrawGui())
            gui->drawGui();

        //update
        windowManager->updateWindow();
    }

    /******************* CLEAN *****************/
    gui->clean();
    loader->clean();
    windowManager->closeWindow();
}

void print(std::string s) {
    std::cout << s << std::endl;
}
