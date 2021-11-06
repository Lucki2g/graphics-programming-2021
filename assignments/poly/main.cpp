//
// Created by Lucki on 05-11-2021.
//

#include "engine/window_manager.h"
#include "engine/model_loader.h"
#include "engine/terrain_renderer.h"
#include "engine/entity_renderer.h"
#include "util/primitives.h"
#include "shaders/static_shader.h"
#include "shaders/terrain_shader.h"
#include "shaders/shader_program.h"
#include "util/obj_reader.h"

const int WIDTH = 720, HEIGHT = 560;

void print(std::string s);

int main () {

    WindowManager* windowManager = new WindowManager(WIDTH, HEIGHT);
    Loader* loader = new Loader();
    ObjReader* objloader = new ObjReader();
    EntityRenderer* entityRenderer = new EntityRenderer();
    TerrainRenderer* terrainRenderer = new TerrainRenderer();

    windowManager->createWindow();

    StaticShader* staticShader = new StaticShader();
    staticShader->Shader::start();
    staticShader->loadProjectionMatrix(windowManager->getProjectionMatrix());
    staticShader->Shader::stop();

    TerrainShader* terrainShader = new TerrainShader();
    terrainShader->Shader::start();
    terrainShader->loadProjectionMatrix(windowManager->getProjectionMatrix());
    terrainShader->Shader::stop();

    //Model* model = objloader->loadObjModel("models/dragon.obj", loader);
    //Entity* entity = new Entity(model, glm::vec3(0, 0, -2), glm::vec3(), 1);
    //entityRenderer->addEntity(entity);

    Light* sun = new Light(glm::vec3(0, 0, -20), glm::vec3(1));

    Terrain* terrain = new Terrain(0, 0, loader);
    terrainRenderer->addTerrain(terrain);

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
        terrainShader->Shader::start();
        terrainShader->loadLight(sun);
        terrainShader->loadViewMatrix(windowManager->getViewMatrix());
        terrainRenderer->render(terrainShader);
        terrainShader->Shader::stop();
        //update
        windowManager->updateWindow();
    }

    loader->clean();
    windowManager->closeWindow();
}

void print(std::string s) {
    std::cout << s << std::endl;
}
