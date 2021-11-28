//
// Created by Lucki on 14-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_FLAT_TERRAIN_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_FLAT_TERRAIN_SHADER_H

#include "shaders/terrain_shader.h"

const char* FLAT_TERRAIN_VERTEX_FILE = "shaders/terrain/flat/terrain.vert";
const char* FLAT_TERRAIN_FRAGMENT_FILE = "shaders/terrain/flat/terrain.frag";

class FlatTerrainShader : public TerrainShader {
    public:
        FlatTerrainShader() : TerrainShader(FLAT_TERRAIN_VERTEX_FILE, FLAT_TERRAIN_FRAGMENT_FILE) { }
};

#endif //ITU_GRAPHICS_PROGRAMMING_NORMAL_TERRAIN_SHADER_H
