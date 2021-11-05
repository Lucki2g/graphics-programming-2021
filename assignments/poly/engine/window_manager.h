//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_WINDOW_MANAGER_H
#define ITU_GRAPHICS_PROGRAMMING_WINDOW_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/****************************
 *          EVENTS          *
 ****************************/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);
}

void cursorCallback(GLFWwindow* window, double posX, double posY) {

}

void framebufferSizeCallback (GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

class WindowManager {

    private:
        GLFWwindow* window;
        int width, height;

    /****************************
     *       CONSTRUCTOR        *
     ****************************/
    public: WindowManager(int w, int h) {
        width = w;
        height = h;
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
};

#endif //ITU_GRAPHICS_PROGRAMMING_WINDOW_MANAGER_H
