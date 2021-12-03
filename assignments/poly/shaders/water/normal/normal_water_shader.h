//
// Created by Lucki on 27-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_NORMAL_WATER_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_NORMAL_WATER_SHADER_H

#include "shaders/water_shader.h"

const char* NORMAL_WATER_VERTEX_FILE = "shaders/water/normal/water.vert";
const char* NORMAL_WATER_FRAGMENT_FILE = "shaders/water/normal/water.frag";

class NormalWaterShader : public WaterShader {
    public:
        NormalWaterShader() : WaterShader(NORMAL_WATER_VERTEX_FILE, NORMAL_WATER_FRAGMENT_FILE){ }
};

#endif
