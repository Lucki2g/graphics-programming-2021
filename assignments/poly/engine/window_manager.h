//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_WINDOW_MANAGER_H
#define ITU_GRAPHICS_PROGRAMMING_WINDOW_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <config.h>
#include "util/glmutils.h"
#include "../entities/camera.h"

/****************************
 *          EVENTS          *
 ****************************/

Camera* camera;
glm::vec2 previous;
bool drawGui = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_SPACE: case GLFW_KEY_ESCAPE:
                drawGui = !drawGui;
                glfwSetInputMode(window, GLFW_CURSOR, drawGui ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
                break;
        }
    }

    /*if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
            case GLFW_KEY_W:
                camera->move(glm::vec3(0, 0, -0.02f));
            case GLFW_KEY_S:
                camera->move(glm::vec3(0, 0, 0.02f));
            case GLFW_KEY_A:
                camera->move(glm::vec3(-0.02f, 0, 0));
            case GLFW_KEY_D:
                camera->move(glm::vec3(0.02f, 0, 0));
        }
    }*/ // Moved to processInput
}

void cursorCallback(GLFWwindow* window, double posX, double posY) {

    if (drawGui)
        return;

    camera->calculate(posX, posY, previous);
    previous = glm::vec2(posX, posY);
}

void framebufferSizeCallback (GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

class WindowManager {

    private:
        GLFWwindow* window;
        int width, height;
        Config* config;

    /****************************
     *       CONSTRUCTOR        *
     ****************************/
    public: WindowManager(Config* config) {
        width = config->width;
        height = config->height;
        camera = new Camera(config->start_position);
        this->config = config;
        previous = glm::vec2(width / 2, height / 2);
    }

    /****************************
     *         METHODS          *
     ****************************/
    void createWindow () {
        if (!glfwInit())
            throw "Unable to initialize GLFW!";

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
        #endif

        window = glfwCreateWindow(width, height, "Land of Polygons", NULL, NULL);
        if (window == NULL)
            throw "Failed to create GLFW window!";

        glfwMakeContextCurrent(window);
        // events
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCursorPosCallback(window, cursorCallback);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            throw "Failed to load GLAD!";

        // enable depth buffer
        // glDepthRange(-1, 1);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);


        // IMGUI init
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 400 core");

        // set provoking vertex
        glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

        glfwShowWindow(window);
    }

    void updateWindow() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void closeWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool shouldClose() {
        return glfwWindowShouldClose(window);
    }

    bool shouldDrawGui() {
        return drawGui;
    }

    glm::mat4 getProjectionMatrix() {
        return glm::perspectiveFov(config->fov, (float) width, (float) height, config->near_plane, config->far_plane);
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(camera->getPosition(), camera->getPosition() + camera->getForward(), glm::vec3(0,1,0));
    }

    void processInput() {
        int press = GLFW_PRESS;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == press && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == press)
            camera->move(config->speed * camera->getForward());
        if (glfwGetKey(window, GLFW_KEY_S) == press)
            camera->move(-config->speed * camera->getForward());
        if (glfwGetKey(window, GLFW_KEY_A) == press)
            camera->move(-config->speed * glm::normalize(glm::cross(camera->getForward(), glm::vec3(0,1,0))));
        if (glfwGetKey(window, GLFW_KEY_D) == press)
            camera->move(config->speed * glm::normalize(glm::cross(camera->getForward(), glm::vec3(0,1,0))));
    }
};

#endif //ITU_GRAPHICS_PROGRAMMING_WINDOW_MANAGER_H
