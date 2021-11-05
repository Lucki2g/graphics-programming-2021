//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_STATIC_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_STATIC_SHADER_H

#include "shader_program.h"

const char* VERTEX_FILE = "shaders/shader.vert";
const char* FRAGMENT_FILE = "shaders/shader.frag";

class StaticShader : public Shader {
    public:
        StaticShader() : Shader(VERTEX_FILE, FRAGMENT_FILE) {
            bindAttributes();
        }

        void bindAttributes() {
            Shader::bindAttribute(0, "position");
        }

        void loadTransformationMatrix(glm::mat4 matrix) {
            Shader::setMat4("transformationMatrix", matrix);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_STATIC_SHADER_H
