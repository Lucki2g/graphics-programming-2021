//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_TERRAIN_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_TERRAIN_SHADER_H

#include "shader_program.h"
#include "../entities/light.h"

class TerrainShader : public Shader {
    public:
        TerrainShader(const char* vertex, const char* fragment) : Shader(vertex, fragment) {
            bindAttributes();
        }
        TerrainShader(const char* vertex, const char* fragment, const char* geometry) : Shader(vertex, fragment, geometry) {
            Shader::bindAttribute(0, "in_position");
            Shader::bindAttribute(2, "in_colour");
        }

        void bindAttributes() {
            Shader::bindAttribute(0, "in_position");
            Shader::bindAttribute(1, "in_normal");
            Shader::bindAttribute(2, "in_colour");
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
            Shader::setVec3("lightColour", light->getColour() * light->getIntensity());
        }

        void loadAmbientLighting(glm::vec3 colour, float intensity, float reflectance) {
            Shader::setVec3("ambientLightColour", colour * intensity);
            Shader::setFloat("ambientReflectance", reflectance);
        }

        void loadDiffuseLighting(float reflectance) {
            Shader::setFloat("diffuseReflectance", reflectance);
        }

        void loadWaterClippingPlane(glm::vec4 plane) {
            Shader::setVec4("waterClippingPlane", plane);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_STATIC_SHADER_H
