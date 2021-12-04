//
// Created by Lucki on 27-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_LOWPOLY_WATER_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_LOWPOLY_WATER_SHADER_H

#include "shaders/water_shader.h"

const char* LOWPOLY_WATER_VERTEX_FILE = "shaders/water/lowpoly/water.vert";
const char* LOWPOLY_WATER_FRAGMENT_FILE = "shaders/water/lowpoly/water.frag";

class LowPolyWaterShader : public WaterShader {
    public:
        LowPolyWaterShader() : WaterShader(LOWPOLY_WATER_VERTEX_FILE, LOWPOLY_WATER_FRAGMENT_FILE){ }
};

#endif
