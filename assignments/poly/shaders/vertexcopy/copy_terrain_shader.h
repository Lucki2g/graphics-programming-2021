//
// Created by Lucki on 14-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_COPY_TERRAIN_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_COPY_TERRAIN_SHADER_H

#include "shaders/terrain_shader.h"

const char* COPY_TERRAIN_VERTEX_FILE = "shaders/vertexcopy/terrain.vert";
const char* COPY_TERRAIN_FRAGMENT_FILE = "shaders/vertexcopy/terrain.frag";

class CopyTerrainShader : public TerrainShader {
    public:
        CopyTerrainShader() : TerrainShader(COPY_TERRAIN_VERTEX_FILE, COPY_TERRAIN_FRAGMENT_FILE) { }
};

#endif //ITU_GRAPHICS_PROGRAMMING_NORMAL_TERRAIN_SHADER_H