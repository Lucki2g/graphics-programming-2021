//
// Created by Lucki on 11-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_CONFIG_H
#define ITU_GRAPHICS_PROGRAMMING_CONFIG_H

#include "util/glmutils.h"
#include "vector"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const int NORMAL = 0;
const int VERTEX_COPY = 1;
const int GEOMETRY = 2;
const int HYBRID = 3;
const int MESH = 4;
const int FLAT = 5;

struct Config {
    /** WINDOW **/
    int width = 1080;
    int height = 720;

    /** CAMERA **/
    float fov = 70.0f;
    float near = 0.1f;
    float far = 1000.0f;
    float speed = 2.0f;

    /** TERRAIN **/
    int terrain_size = 100;
    float size = 200.0f;
    std::vector<glm::vec3> pallet = {
            glm::vec3(201, 178, 99),
            glm::vec3(135, 184, 82),
            glm::vec3(80, 171, 93),
            glm::vec3(120, 120, 120),
            glm::vec3(200, 200, 210)
    };
    float spread = 0.45f;
    float half_spread = spread / 2.0;
    float part = 1.0f / (pallet.size() - 1);

    float roughness = 0.35f;
    float amplitude = 10.0f;
    int octaves = 3;
    float frequency = 0.1f;
    int generationSetting = NORMAL;

    int HEIGHTMAP = 0;
    int PERLIN = 1;

    /** LIGHTING **/
    // ambient light
    glm::vec3 ambientLightColour = {1.0f, 1.0f, 1.0f};
    float ambientLightIntensity = 0.2f;
    // universal
    glm::vec3 lightDirection = { 0.5f, -1.0f, 0.5f };
    // light 1
    glm::vec3 lightColour = {1.0f, 1.0f, 1.0f};
    glm::vec3 lightPosition = {-terrain_size / 2, 300.0f, -terrain_size / 2};
    float lightIntensity = 1.0f;
    // material
    float ambientReflectance = 0.5f;
    float diffuseReflectance = 1.0f;

};

#endif //ITU_GRAPHICS_PROGRAMMING_CONFIG_H
