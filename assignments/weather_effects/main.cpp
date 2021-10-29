#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <vector>
#include <chrono>

#include "shader.h"
#include "glmutils.h"
#include "primitives.h"
#include <math.h>

// Debug mode
#define __DEBUG__ false

// NOTICE: Set the RAIN variable to
//         false to render snow, and 
//         to true to render rain
const bool RAIN = true;

// Struct used for both solid scene geometry
// and particle effects
struct SceneObject
{
    unsigned int VAO;
    unsigned int vertexCount;

    void draw(int drawType) const
    {
        glBindVertexArray(VAO);
        glDrawElements(drawType, vertexCount, GL_UNSIGNED_INT, 0);
    }
};

// Function declarations
unsigned int createArrayBuffer(const std::vector<float> &array);
unsigned int createElementArrayBuffer(const std::vector<unsigned int> &array);
unsigned int createVertexArray(const std::vector<float> &positions, const std::vector<float> &colors, const std::vector<unsigned int> &indices);

// GLFW and input
void cursorInRange(float screenX, float screenY, int screenW, int screenH, float min, float max, float &x, float &y);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void cursor_input_callback(GLFWwindow* window, double posX, double posY);

// Draw functions
void drawObjects(glm::mat4 scale, glm::mat4 viewProjection);
void drawParticles(glm::mat4 model);
void drawCube(glm::mat4 model);
void drawFloor(glm::mat4 model);
void generateParticles(std::vector<float> &particleVertices, 
                       std::vector<unsigned int> &particleIndices, 
                       std::vector<float> &particleColors,
                       int numberOfParticles);
void generateOffsets(std::vector<glm::vec3> &gravityOffsets, 
                     std::vector<glm::vec3> &windOffsets, 
                     std::vector<glm::vec3> &randomOffsets,
                     int numberOfOffsets);
void setup();

// Screen settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// Global object vars
SceneObject cube;
SceneObject floorObj;
SceneObject particleBox;

// Box size
const float BOX_SIZE = 30.f;

// Global variables we will use to store our objects, shaders, and active shader
std::vector<SceneObject> sceneObjects;
std::vector<Shader> shaderPrograms;
Shader* activeShader;

// Global camera settings
glm::vec3 cameraPos   = glm::vec3(0.0f, 1.6f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

float currentTime, cameraSpeed = .15f, rotationGain = 30.0f;

int main()
{
    // glfw: Initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif

    // GLFW window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment - Weather Effects", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_input_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD: Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set up shader programs and VAO globals
    setup();

    // Setting up the z-buffer
    glDepthRange(-1, 1); // The NDC's a left-handed coordinate system with the camera pointing towards -z
    glEnable(GL_DEPTH_TEST); // Enable z-buffer depth test
    glDepthFunc(GL_LESS); // Draws fragments that are closer to the screen in NDC

    // Enabling the built-in gl_PointSize
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    // Render every interval seconds
    float interval = 0.02f;
    auto begin = std::chrono::high_resolution_clock::now();

    std::vector<glm::vec3> gravityOffsets;
    std::vector<glm::vec3> windOffsets;
    std::vector<glm::vec3> randomOffsets;
    
    int simulations = (RAIN) ? 5 : 10;
    generateOffsets(gravityOffsets, windOffsets, randomOffsets, simulations);

    // Render loop
    glm::mat4 viewProjectionPrev;
    bool firstIteration = true; // True until the loop body has executed once
                                // and there exists a previous frame
    while (!glfwWindowShouldClose(window)) {
        auto frameStart = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> appTime = frameStart - begin;
        currentTime = appTime.count();

        processInput(window);

        glClearColor(0.0f, 0.15f, 0.30f, 1.0f); // Sky blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Setting scale and view projection
        glm::mat4 scale = glm::scale(1.f, 1.f, 1.f);
        glm::mat4 projection = glm::perspectiveFov(70.0f, (float) SCR_WIDTH, (float) SCR_HEIGHT, .01f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 viewProjection = projection * view;
        
        if (firstIteration) {
            viewProjectionPrev = viewProjection;
            firstIteration = false;
        }
        
        // Drawing objects
        activeShader = &shaderPrograms[0];
        glUseProgram(activeShader->ID);
        drawObjects(scale, viewProjection);

        // Drawing particles
        activeShader = (RAIN) ? &shaderPrograms[2] : &shaderPrograms[1];
        glUseProgram(activeShader->ID);
        activeShader->setFloat("boxSize", BOX_SIZE);
        activeShader->setFloat("currentTime", currentTime);

        // Enabling alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

        for (int i = 0; i < simulations; i++) {
            glm::vec3 gravityOffset = gravityOffsets[i] * currentTime;
            glm::vec3 windOffset = windOffsets[i] * currentTime;
            
            if (RAIN) {
                float gravityMultiplier = 10.f;
                gravityOffset = gravityOffset * gravityMultiplier;
                windOffset = glm::vec3(0.f, 0.f, 0.f);
            }

            glm::vec3 offsets = gravityOffset + windOffset + randomOffsets[i];
            glm::vec3 forwardOffset = cameraFront * 12.f;
            offsets -= cameraPos + forwardOffset + (BOX_SIZE / 2);
            float xOffset = (float) fmod(offsets.x, BOX_SIZE);
            float yOffset = (float) fmod(offsets.y, BOX_SIZE);
            float zOffset = (float) fmod(offsets.z, BOX_SIZE);
            offsets = glm::vec3(xOffset, yOffset, zOffset);

            // Setting uniforms
            activeShader->setVec3("offsets", offsets);
            activeShader->setVec3("cameraPos", cameraPos);
            activeShader->setVec3("cameraFront", cameraFront);
            activeShader->setVec3("forwardOffset", forwardOffset);
            activeShader->setMat4("modelPrev", viewProjectionPrev);
            activeShader->setVec3("velocity", gravityOffsets[i]);
            
            drawParticles(viewProjection);
        }
        viewProjectionPrev = viewProjection;

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Disabling alpha blending again
        glDisable(GL_BLEND);

        std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now()-frameStart;
        while (interval > elapsed.count()) {
            elapsed = std::chrono::high_resolution_clock::now() - frameStart;
        }
    }

    glfwTerminate();
    return 0;
}

// Shader initialization and object creation
void setup()
{
    // Object and particle (snow and rain) shader programs
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/shader.frag"));
    shaderPrograms.push_back(Shader("shaders/snow.vert", "shaders/snow.frag"));
    shaderPrograms.push_back(Shader("shaders/rain.vert", "shaders/rain.frag"));
    
    // Objects
    activeShader = &shaderPrograms[0];
    floorObj.VAO = createVertexArray(floorVertices, floorColors, floorIndices);
    floorObj.vertexCount = (int) floorIndices.size();
    cube.VAO = createVertexArray(cubeVertices, cubeColors, cubeIndices);
    cube.vertexCount = (int) cubeIndices.size();

    // Particles
    activeShader = (RAIN) ? &shaderPrograms[2] : &shaderPrograms[1];
    
    std::vector<float> particleVertices;
    std::vector<unsigned int> particleIndices;
    std::vector<float> particleColors;
    int numberOfParticles = 10000;
    
    generateParticles(particleVertices, particleIndices, particleColors, numberOfParticles);
    particleBox.VAO = createVertexArray(particleVertices, particleColors, particleIndices);
    particleBox.vertexCount = (int) particleIndices.size();
}

void drawObjects(glm::mat4 scale, glm::mat4 viewProjection)
{
    drawFloor(viewProjection);
    drawCube(viewProjection * glm::translate(2.0f, 1.f, 2.0f) * glm::rotateY(glm::half_pi<float>()) * scale);
    drawCube(viewProjection * glm::translate(-2.0f, 1.f, -2.0f) * glm::rotateY(glm::quarter_pi<float>()) * scale);
}

void drawFloor(glm::mat4 model)
{
    activeShader->setMat4("model", model);
    floorObj.draw(GL_TRIANGLES);
}

void drawCube(glm::mat4 model)
{
    activeShader->setMat4("model", model);
    cube.draw(GL_TRIANGLES);
}

void drawParticles(glm::mat4 model)
{
    activeShader->setMat4("model", model);
    int drawType = (RAIN) ? GL_LINES : GL_POINTS;
    particleBox.draw(drawType);
}

void generateParticles(std::vector<float> &particleVertices,
                       std::vector<unsigned int> &particleIndices,
                       std::vector<float> &particleColors,
                       int numberOfParticles)
{
    // If simulating rain, each vertex position and color is pushed twice
    int count = (RAIN) ? 2 : 1;

    for (int particle = 0; particle < numberOfParticles; particle += count) {
        // Vertex coords
        float xVal = BOX_SIZE + (float) rand() / ((float) RAND_MAX / BOX_SIZE);
        float yVal = BOX_SIZE + (float) rand() / ((float) RAND_MAX / BOX_SIZE);
        float zVal = BOX_SIZE + (float) rand() / ((float) RAND_MAX / BOX_SIZE);

        for (int i = 0; i < count; i++) {
            // Vertex position
            particleVertices.push_back(xVal);
            particleVertices.push_back(yVal);
            particleVertices.push_back(zVal);

            // Vertex color
            for (int color = 0; color < 4; color++) {
                particleColors.push_back(1.f);
            }

            // Vertex index
            particleIndices.push_back(particle + i);
        }
    }
}

void generateOffsets(std::vector<glm::vec3> &gravityOffsets,
                     std::vector<glm::vec3> &windOffsets,
                     std::vector<glm::vec3> &randomOffsets,
                     int numberOfOffsets)
{
    for (int i = 0; i < numberOfOffsets; i++) {
        float gravY = -1.f + ((float) rand() / (RAND_MAX / .5f));
        glm::vec3 randomGravity = glm::vec3(0.f, gravY, 0.f);

        float randX = (float) rand() / (float) RAND_MAX;
        float randY = (float) rand() / (float) RAND_MAX;
        float randZ = (float) rand() / (float) RAND_MAX;
        glm::vec3 randomRandom = glm::vec3(randX, randY, randZ);

        float windX = -1.f + ((float) rand() / (RAND_MAX / 2.f));
        float windZ = -1.f + ((float) rand() / (RAND_MAX / 2.f));
        glm::vec3 randomWind = glm::vec3(randX, 0.f, randZ);

        gravityOffsets.push_back(randomGravity);
        windOffsets.push_back(randomWind);
        randomOffsets.push_back(randomRandom);
        
        #if __DEBUG__
            std::cout << "Offsets generated by generateOffsets:" << std::endl;
            std::cout << randomGravity << " " << randomWind << " " << randomRandom << std::endl;
        #endif
    }
}

unsigned int createVertexArray(const std::vector<float> &positions,
                               const std::vector<float> &colors, 
                               const std::vector<unsigned int> &indices)
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // Bind the VAO
    glBindVertexArray(VAO);

    // Set the vertex shader attribute "pos"
    createArrayBuffer(positions); // Create and bind the VBO
    int posAttributeLocation = glGetAttribLocation(activeShader->ID, "pos");
    glEnableVertexAttribArray(posAttributeLocation);
    glVertexAttribPointer(posAttributeLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Set the vertex shader attribute "color"
    createArrayBuffer(colors); // Create and bind the VBO
    int colorAttributeLocation = glGetAttribLocation(activeShader->ID, "color");
    glEnableVertexAttribArray(colorAttributeLocation);
    glVertexAttribPointer(colorAttributeLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // Create and bind the EBO
    createElementArrayBuffer(indices);

    return VAO;
}

unsigned int createArrayBuffer(const std::vector<float> &array)
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, array.size() * sizeof(GLfloat), &array[0], GL_STATIC_DRAW);

    return VBO;
}

unsigned int createElementArrayBuffer(const std::vector<unsigned int> &array)
{
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, array.size() * sizeof(unsigned int), &array[0], GL_STATIC_DRAW);

    return EBO;
}

void cursorInRange(float screenX, float screenY, int screenW, int screenH, float min, float max, float &x, float &y)
{
    float sum = max - min;
    float xInRange = (float) screenX / (float) screenW * sum - sum/2.0f;
    float yInRange = (float) screenY / (float) screenH * sum - sum/2.0f;
    x = xInRange;
    y = -yInRange;
}

void cursor_input_callback(GLFWwindow* window, double posX, double posY)
{
    int screenW, screenH;
    glfwGetWindowSize(window, &screenW, &screenH);
    glm::vec2 cursorPosition(0.0f);
    cursorInRange((float) posX, (float) posY, screenW, screenH, -1.0f, 1.0f, cursorPosition.x, cursorPosition.y);

    static glm::vec2 lastCursorPosition = cursorPosition;
    glm::vec2 positionDiff = cursorPosition - lastCursorPosition;

    static float rotationAroundVertical = 0;
    static float rotationAroundLateral  = 0;
    if (glm::dot(positionDiff, positionDiff) > 1e-5f){
        cameraFront = glm::vec3 (0, 0, -1);
        // rotate the forward vector around the Y axis, notices that w is set to 0 since it is a vector
        rotationAroundVertical += glm::radians(-positionDiff.x * rotationGain);
        cameraFront = glm::rotateY(rotationAroundVertical) * glm::vec4(cameraFront, 0.0f);

        // rotate the forward vector around the lateral axis
        rotationAroundLateral += glm::radians(positionDiff.y * rotationGain);
        // we need to clamp the range of the rotation, otherwise forward and Y axes get parallel
        rotationAroundLateral  = glm::clamp(rotationAroundLateral, -glm::half_pi<float>() * 0.9f, glm::half_pi<float>() * 0.9f);

        glm::vec3 lateralAxis  = glm::cross(cameraFront, glm::vec3(0, 1,0));

        cameraFront = glm::rotate(rotationAroundLateral, lateralAxis) * glm::vec4(cameraFront, 0);

        lastCursorPosition = cursorPosition;
    }
}

void processInput(GLFWwindow *window) 
{
    // Exits on pressing the Escape key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 feetOnTheGround = glm::normalize(glm::vec3(cameraFront.x, 0.f, cameraFront.z));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * feetOnTheGround;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * feetOnTheGround;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraSpeed * (glm::cross(feetOnTheGround, 
                                    glm::vec3(0.f, 1.f, 0.f)));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraSpeed * (glm::cross(feetOnTheGround,
                                    glm::vec3(0.f, 1.f, 0.f)));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}