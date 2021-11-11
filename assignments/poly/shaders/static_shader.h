//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_STATIC_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_STATIC_SHADER_H

#include "shader_program.h"
#include "../entities/light.h"

const char* VERTEX_FILE = "shaders/shader.vert";
const char* FRAGMENT_FILE = "shaders/shader.frag";

class StaticShader : public Shader {
    public:
        StaticShader() : Shader(VERTEX_FILE, FRAGMENT_FILE) {
            bindAttributes();
        }

        void bindAttributes() {
            Shader::bindAttribute(0, "in_position");
            Shader::bindAttribute(1, "in_normal");
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

        void loadLight(Light* light) {
            Shader::setVec3("lightPosition", light->getPosition());
            Shader::setVec3("lightColour", light->getColour());
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_STATIC_SHADER_H
