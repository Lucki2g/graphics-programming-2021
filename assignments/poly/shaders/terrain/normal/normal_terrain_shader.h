//
// Created by Lucki on 14-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_NORMAL_TERRAIN_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_NORMAL_TERRAIN_SHADER_H

#include "shaders/terrain_shader.h"

const char* NORMAL_TERRAIN_VERTEX_FILE = "shaders/terrain/normal/terrain.vert";
const char* NORMAL_TERRAIN_FRAGMENT_FILE = "shaders/terrain/normal/terrain.frag";

class NormalTerrainShader : public TerrainShader {
    public:
        NormalTerrainShader() : TerrainShader(NORMAL_TERRAIN_VERTEX_FILE, NORMAL_TERRAIN_FRAGMENT_FILE) { }
};

#endif //ITU_GRAPHICS_PROGRAMMING_NORMAL_TERRAIN_SHADER_H
