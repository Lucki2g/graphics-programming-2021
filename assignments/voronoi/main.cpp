#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>

#include <iostream>
#include <vector>
#include <math.h>

// structure to hold the info necessary to render an object
struct SceneObject {
    unsigned int VAO, EBO;      // vertex array object handle
    unsigned int vertexCount;   // number of vertices in the object
    float r, g, b;              // for object color
    float x, y;                 // for position offset

    std::vector<float> getColor() { return {r, g, b}; }
};

// declaration of the function you will implement in voronoi 1.1
SceneObject instantiateCone(float r, float g, float b, float offsetX, float offsetY);
// mouse, keyboard and screen reshape glfw callbacks
void button_input_callback(GLFWwindow* window, int button, int action, int mods);
void key_input_callback(GLFWwindow* window, int button, int other,int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// global variables we will use to store our objects, shaders, and active shader
std::vector<SceneObject> sceneObjects;
std::vector<Shader> shaderPrograms;
Shader* activeShader;
// uniform variables
GLint offsetPos, coneColor;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment - Voronoi Diagram", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // setup frame buffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // setup input callbacks
    glfwSetMouseButtonCallback(window, button_input_callback); // NEW!
    glfwSetKeyCallback(window, key_input_callback); // NEW!

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // NEW!
    // build and compile the shader programs
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/color.frag"));
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/distance.frag"));
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/distance_color.frag"));
    activeShader = &shaderPrograms[0];

    coneColor = glGetUniformLocation(shaderPrograms[0].ID, "coneColor");
    offsetPos = glGetUniformLocation(shaderPrograms[0].ID, "offsetPos");

    // NEW!
    // set up the z-buffer
    glDepthRange(1,-1); // make the NDC a right handed coordinate system, with the camera pointing towards -z
    glEnable(GL_DEPTH_TEST); // turn on z-buffer depth test
    glDepthFunc(GL_LESS); // draws fragments that are closer to the screen in NDC

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // notice that now we are clearing two buffers, the color and the z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the cones
        glUseProgram(activeShader->ID);

        // TODO voronoi 1.3
        // Iterate through the scene objects, for each object:
        // - bind the VAO; set the uniform variables; and draw
        for (SceneObject so : sceneObjects) {
            glBindVertexArray(so.VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, so.EBO);

            if (activeShader != &shaderPrograms[1]) glUniform3f(coneColor, so.r, so.g, so.b);
            glUniform2f(offsetPos, so.x, so.y);

            glDrawElements(GL_TRIANGLES, so.vertexCount, GL_UNSIGNED_INT, 0);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}


// creates a cone triangle mesh, uploads it to openGL and returns the VAO associated to the mesh
SceneObject instantiateCone(float r, float g, float b, float offsetX, float offsetY){
    // TODO voronoi 1.1
    // (exercises 1.7 and 1.8 can help you with implementing this function)

    // Create an instance of a SceneObject,
    SceneObject sceneObject{};

    // you will need to store offsetX, offsetY, r, g and b in the object.
    sceneObject.r = r;
    sceneObject.g = g;
    sceneObject.b = b;
    sceneObject.x = offsetX;
    sceneObject.y = offsetY;

    // Build the geometry into an std::vector<float> or float array.
    // TODO: calculate distance and save in z value of every vertex
    /* The difference is that the cone should also hold meaningful 'z' coordinate information,
     * which describes the distance of each vertex from the virtual camera in the range [-1, 1].
     * We want the center of the cone to be close to the camera,
     * and the base of the cone to be far from the camera.
     * The radius of the cone should be sufficient to cover all the drawing area in the NDC xy plane
     * (i.e. radius >= sqrt(2*2+2*2)).
     * */
    int count = 32;
    float PI = 3.14159265358979323846;
    std::vector<float> data {
            0.0f, 0.0f, 1.0f,    // center
            0.5f, 0.5f, 0.5f     // center color
    };
    float interval = (2 * PI) / (float) count;
    for (int i = 0; i < count; i++) {
        // vertex
        data.push_back(cos(i * interval) * 2);
        data.push_back(sin(i * interval) * 2);
        data.push_back(-1.0f);
    }

    std::vector<unsigned int> indices;
    for (int i = 0; i < count - 1; i++) {
        indices.push_back(0);
        indices.push_back(((i + 1) * 2));
        indices.push_back(((i + 2) * 2));
    }
    // last vertex
    indices.push_back(0);
    indices.push_back(count);
    indices.push_back(2);

    // Store the number of vertices in the mesh in the scene object.
    sceneObject.vertexCount = count * 3;

    // Declare and generate a VAO and VBO (and an EBO if you decide the work with indices).
    // Bind and set the VAO and VBO (and optionally a EBO) in the correct order.
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

    // Set the position attribute pointers in the shader.
    int posSize = 3;
    int posAttributeLocation = glGetAttribLocation(shaderPrograms[0].ID, "aPos");
    if (posAttributeLocation < 0) std::cout << "pos not found ..." << std::endl;
    glVertexAttribPointer(posAttributeLocation, posSize, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posAttributeLocation);

    /*int colorSize = 3;
    int colorAttributeLocation = glGetAttribLocation(shaderPrograms[0].ID, "aColor");
    if (colorAttributeLocation < 0) std::cout << "color not found ..." << std::endl;
    glVertexAttribPointer(colorAttributeLocation, colorSize, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(colorAttributeLocation);*/

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sceneObject.vertexCount, &indices[0], GL_STATIC_DRAW);

    // Store the VAO handle in the scene object.
    sceneObject.VAO = VAO;
    sceneObject.EBO = EBO;

    // unbind VAO
    glBindVertexArray(0);

    // 'return' the scene object for the cone instance you just created.
    return sceneObject;
}

// glfw: called whenever a mouse button is pressed
void button_input_callback(GLFWwindow* window, int button, int action, int mods){
    // TODO voronoi 1.2
    // get screen size and click coordinates
    double xPos, yPos;
    int xScreen, yScreen;
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwGetWindowSize(window, &xScreen, &yScreen);

    // convert from screen space to normalized display coordinates
    float xNdc = (float) xPos/(float) xScreen * 2.0f -1.0f;
    float yNdc = (float) yPos/(float) yScreen * 2.0f -1.0f;
    yNdc = -yNdc;

    // Test button press, see documentation at: https://www.glfw.org/docs/latest/input_guide.html#input_mouse_button
    // If a left mouse button press was detected, call instantiateCone:
    // - Push the return value to the back of the global 'vector<SceneObject> sceneObjects'.
    // - The click position should be transformed from screen coordinates to normalized device coordinates,
    //   to obtain the offset values that describe the position of the object in the screen plane.
    // - A random value in the range [0, 1] should be used for the r, g and b variables.
    if (action == GLFW_PRESS) {
        float color[3] = {
            (rand()%256) / 256.0f,
            (rand()%256) / 256.0f,
            (rand()%256) / 256.0f
        };
        SceneObject cone = instantiateCone(color[0], color[1], color[2], xNdc, yNdc);
        sceneObjects.push_back(cone);
    }
}

// glfw: called whenever a keyboard key is pressed
void key_input_callback(GLFWwindow* window, int button, int other, int action, int mods){
    // TODO voronoi 1.4
    // Set the activeShader variable by detecting when the keys 1, 2 and 3 were pressed;
    // see documentation at https://www.glfw.org/docs/latest/input_guide.html#input_keyboard
    // Key 1 sets the activeShader to &shaderPrograms[0];
    //   and so on.
    if (button == GLFW_KEY_1 && action == GLFW_PRESS) {
        activeShader = &shaderPrograms[0];
        offsetPos = glGetUniformLocation(shaderPrograms[0].ID, "offsetPos");
        coneColor = glGetUniformLocation(shaderPrograms[0].ID, "coneColor");
        std::cout << "(" << offsetPos << ", " << coneColor << ")" << std::endl;
    } else if (button == GLFW_KEY_2 && action == GLFW_PRESS) {
        activeShader = &shaderPrograms[1];
        offsetPos = glGetUniformLocation(shaderPrograms[1].ID, "offsetPos");
        std::cout << "(" << offsetPos << ", " << coneColor << ")" << std::endl;
    } else if (button == GLFW_KEY_3 && action == GLFW_PRESS) {
        activeShader = &shaderPrograms[2];
        offsetPos = glGetUniformLocation(shaderPrograms[2].ID, "offsetPos");
        coneColor = glGetUniformLocation(shaderPrograms[2].ID, "coneColor");
        std::cout << "(" << offsetPos << ", " << coneColor << ")" << std::endl;
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}