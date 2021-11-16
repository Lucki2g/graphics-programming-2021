//
// Created by Lucki on 16-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_WATER_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_WATER_SHADER_H

#include "../shader_program.h"

const char* WATER_VERTEX_FILE = "shaders/water/water.vert";
const char* WATER_FRAGMENT_FILE = "shaders/water/water.frag";

class WaterShader : public Shader {
    public:
        WaterShader() : Shader(WATER_VERTEX_FILE, WATER_FRAGMENT_FILE) {
            bindAttributes();
        }

        void loadTransformationMatrix(glm::mat4 matrix) {
            Shader::setMat4("transformationMatrix", matrix);
        }

        void loadProjectionMatrix(glm::mat4 matrix) {
            Shader::setMat4("projectionMatrix", matrix);
        }

        void loadViewMatrix(glm::mat4 matrix) {
            Shader::setMat4("viewMatrix", matrix);
        }

    private:
        void bindAttributes() {
            Shader::bindAttribute(0, "in_position");
        }
};
#endif //ITU_GRAPHICS_PROGRAMMING_WATER_SHADER_H
