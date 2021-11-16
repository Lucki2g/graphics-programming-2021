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
#include <shaders/geometry/geomoetry_terrain_shader.h>
#include <shaders/flat/flat_terrain_shader.h>

void print(std::string s);

int main () {
    /******************* SETUP *****************/
    Config* config = new Config();
    WindowManager* windowManager = new WindowManager(config);
    Loader* loader = new Loader();
    ObjReader* objloader = new ObjReader();
    EntityRenderer* entityRenderer = new EntityRenderer();
    TerrainRenderer* terrainRenderer = new TerrainRenderer();
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
    normalShader->Shader::stop();

    CopyTerrainShader* copyShader = new CopyTerrainShader();
    copyShader->Shader::start();
    copyShader->loadProjectionMatrix(windowManager->getProjectionMatrix());
    copyShader->Shader::stop();

    GeometryTerrainShader* geoShader = new GeometryTerrainShader();
    geoShader->Shader::start();
    geoShader->loadProjectionMatrix(windowManager->getProjectionMatrix());
    geoShader->Shader::stop();

    FlatTerrainShader* flatShader = new FlatTerrainShader();
    flatShader->Shader::start();
    flatShader->loadProjectionMatrix(windowManager->getProjectionMatrix());
    flatShader->Shader::stop();

    /******************* MODELS & TERRAIN *****************/
    /*Model* model = objloader->loadObjModel("models/dragon.obj", loader);
    Entity* entity = new Entity(model, glm::vec3(0, 0, -2), glm::vec3(), 1);
    entityRenderer->addEntity(entity);*/

    Model* cubeModel = loader->loadToVao(cubeVertices, cubeColors, cubeNormals, cubeIndices);
    Entity* cubeEntity = new Entity(cubeModel, config->lightPosition, glm::vec3(), 1);
    entityRenderer->addEntity(cubeEntity);

    // std::cout << config->lightPosition.x << "," << config->lightPosition.y << "," << config->lightPosition.z << std::endl;
    Light* sun = new Light(config->lightPosition, config->lightColour, config->lightIntensity);

    Terrain* terrain1 = new Terrain(0, 0, loader, colourGenerator, config, NORMAL);
    terrainRenderer->addTerrain(terrain1, NORMAL);
    terrainRenderer->addTerrain(terrain1, MESH);
    terrainRenderer->addTerrain(terrain1, FLAT);

    Terrain* terrain2 = new Terrain(0, 0, loader, colourGenerator, config, VERTEX_COPY);
    terrainRenderer->addTerrain(terrain2, VERTEX_COPY);

    Terrain* terrain3 = new Terrain(0, 0, loader, colourGenerator, config, GEOMETRY);
    terrainRenderer->addTerrain(terrain3, GEOMETRY);

    Gui* gui = new Gui(config, sun, cubeEntity);

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
                normalShader->TerrainShader::loadLightDirection(config->lightDirection);
                normalShader->TerrainShader::loadViewMatrix(windowManager->getViewMatrix());
                normalShader->loadAmbientLighting(config->ambientLightColour, config->ambientLightIntensity, config->ambientReflectance);
                normalShader->loadDiffuseLighting(config->diffuseReflectance);
                terrainRenderer->render(normalShader, config->generationSetting);
                normalShader->TerrainShader::Shader::stop();
                break;
            case FLAT:
                flatShader->TerrainShader::Shader::start();
                flatShader->TerrainShader::loadLight(sun);
                flatShader->TerrainShader::loadViewMatrix(windowManager->getViewMatrix());
                flatShader->loadAmbientLighting(config->ambientLightColour, config->ambientLightIntensity, config->ambientReflectance);
                flatShader->loadDiffuseLighting(config->diffuseReflectance);
                terrainRenderer->render(flatShader, config->generationSetting);
                flatShader->TerrainShader::Shader::stop();
                break;
            case VERTEX_COPY:
                copyShader->Shader::start();
                copyShader->loadLight(sun);
                copyShader->loadViewMatrix(windowManager->getViewMatrix());
                copyShader->loadAmbientLighting(config->ambientLightColour, config->ambientLightIntensity, config->ambientReflectance);
                copyShader->loadDiffuseLighting(config->diffuseReflectance);
                terrainRenderer->render(copyShader, config->generationSetting);
                copyShader->Shader::stop();
                break;
            case GEOMETRY:
                geoShader->Shader::start();
                geoShader->loadLight(sun);
                geoShader->loadViewMatrix(windowManager->getViewMatrix());
                geoShader->loadAmbientLighting(config->ambientLightColour, config->ambientLightIntensity, config->ambientReflectance);
                geoShader->loadDiffuseLighting(config->diffuseReflectance);
                terrainRenderer->render(geoShader, config->generationSetting);
                geoShader->Shader::stop();
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
