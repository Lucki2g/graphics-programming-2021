//
// Created by Lucki on 14-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_GEOMETRY_TERRAIN_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_GEOMETRY_TERRAIN_SHADER_H

#include "shaders/terrain_shader.h"

const char* GEOMETRY_TERRAIN_VERTEX_FILE = "shaders/terrain/geometry/terrain.vert";
const char* GEOMETRY_TERRAIN_FRAGMENT_FILE = "shaders/terrain/geometry/terrain.frag";
const char* GEOMETRY_TERRAIN_GEOMETRY_FILE = "shaders/terrain/geometry/terrain.glsl";

class GeometryTerrainShader : public TerrainShader {
    public:
        GeometryTerrainShader() : TerrainShader(GEOMETRY_TERRAIN_VERTEX_FILE, GEOMETRY_TERRAIN_FRAGMENT_FILE, GEOMETRY_TERRAIN_GEOMETRY_FILE) { }
};

#endif //ITU_GRAPHICS_PROGRAMMING_NORMAL_TERRAIN_SHADER_H