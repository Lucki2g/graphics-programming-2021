//
// Created by Lucki on 05-11-2021.
//

#include "engine/window_manager.h"
#include "engine/model_loader.h"
#include "engine/renderer.h"
#include "primitives.h"
#include "shaders/static_shader.h"
#include "shaders/shader_program.h"

const int WIDTH = 720, HEIGHT = 560;

void print(std::string s);

int main () {

    WindowManager* windowManager = new WindowManager(WIDTH, HEIGHT);
    Loader* loader = new Loader();
    Renderer* renderer = new Renderer();

    windowManager->createWindow();

    StaticShader* staticShader = new StaticShader();
    staticShader->Shader::start();
    staticShader->loadProjectionMatrix(windowManager->getProjectionMatrix());
    staticShader->Shader::stop();

    const std::vector<float> vertices = {
            -0.5f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
    };
    const std::vector<unsigned int> indices = {
            0, 1, 3,
            3, 1, 2
    };

    Model* cube = loader->loadToVao(vertices, indices);
    Model* quad = loader->loadToVao(vertices, indices);
    Entity* entCube = new Entity(cube, glm::vec3(0.5, 0.5, -1), glm::vec3(0), 1);
    Entity* ent2 = new Entity(quad, glm::vec3(-1,0,-1), glm::vec3(0), 1);


    print("loop");
    while (!windowManager->shouldClose()) {
        // prepare
        renderer->prepare();
        // process input
        windowManager->processInput();
        // start shader
        staticShader->Shader::start();
        staticShader->loadViewMatrix(windowManager->getViewMatrix());
        // render
        renderer->render(ent2, staticShader);
        renderer->render(entCube, staticShader);
        staticShader->Shader::stop();
        //update
        windowManager->updateWindow();
    }

    loader->clean();
    windowManager->closeWindow();
}

void print(std::string s) {
    std::cout << s << std::endl;
}
