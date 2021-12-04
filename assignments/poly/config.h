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
const int MESH = 3;
const int FLAT = 4;
const int FLAT_S = 5;
const int EQUILINOX = 6;

struct Config {
    /** WINDOW **/
    int width = 1080;
    int height = 720;
    glm::vec3 backgroundColour = {1.0f, 1.0f, 1.0f};
    bool showFbos = false;

    /** CAMERA **/
    float fov = 70.0f;
    float near_plane = 0.1f;
    float far_plane = 1000.0f;
    float speed = 0.50f;
    glm::vec3 start_position = glm::vec3(0, 50, 0);

    /** TERRAIN **/
    int terrain_size = 400;
    // float size = 200.0f;
    std::vector<glm::vec3> pallet = {
            // beach
            glm::vec3(253,252,220),
            glm::vec3(254, 217, 183),
            // grass
            glm::vec3(178, 227, 140),
            glm::vec3(150, 218, 139),
            // mountain
            glm::vec3(206, 212, 218),
            glm::vec3(73, 80, 87),
            // snow
            glm::vec3(248, 249, 250)
    };
    float spread = 0.45f;
    float half_spread = spread / 2.0;
    float part = 1.0f / (pallet.size() - 1);

    float amplitude = 12.0f;
    int octaves = 10;
    float frequency = 0.035f;
    int generationSetting = HEIGHTMAP;

    int HEIGHTMAP = 0;
    int PERLIN = 1;

    /** LIGHTING **/
    // ambient light
    glm::vec3 ambientLightColour = {1.0f, 0.8f, 0.8f};
    float ambientLightIntensity = 0.3f;
    // universal
    glm::vec3 lightDirection = { 0.5f, -1.0f, 0.5f };
    // light 1
    glm::vec3 lightColour = {1.0f, 1.0f, 1.0f};
    glm::vec3 lightPosition = {-terrain_size / 2, 300.0f, -terrain_size / 2};
    float lightIntensity = 1.0f;
    // material
    float ambientReflectance = 0.3f;
    float diffuseReflectance = 0.8f;

    /** WATER **/
    int water_height = -1.0f;
    float wave_speed = 0.004f;
    const int reflection_width = width / 2;//320;
    const int reflection_height = height / 2;//180;
    const int refraction_width = width / 2;//1280;
    const int refraction_height = height / 2;//720;

};

#endif //ITU_GRAPHICS_PROGRAMMING_CONFIG_H
