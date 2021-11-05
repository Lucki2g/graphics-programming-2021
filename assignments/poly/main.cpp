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

    print("setup");
    WindowManager* windowManager = new WindowManager(WIDTH, HEIGHT);
    Loader* loader = new Loader();
    Renderer* renderer = new Renderer();

    print("create");
    windowManager->createWindow();

    print("shader");
    StaticShader* staticShader = new StaticShader();

    print("quad");
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
    Entity* entCube = new Entity(cube, glm::vec3(-1, 0, 0), 1);

    print("loop");
    while (!windowManager->shouldClose()) {
        // prepare
        renderer->prepare();
        //render
        staticShader->Shader::start();
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
