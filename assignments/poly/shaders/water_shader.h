//
// Created by Lucki on 16-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_WATER_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_WATER_SHADER_H

#include "shader_program.h"

class WaterShader : public Shader {
    public:
        WaterShader(const char* vertex, const char* fragment) : Shader(vertex, fragment) {
            bindAttributes();
        }

        void loadProjectionMatrix(glm::mat4 matrix) {
            Shader::setMat4("projectionMatrix", matrix);
        }

        void loadViewMatrix(glm::mat4 matrix) {
            Shader::setMat4("viewMatrix", matrix);
        }

        void loadTextures() {
            Shader::setInt("reflectionTexture", 0);
            Shader::setInt("refractionTexture", 1);
            Shader::setInt("depthTexture", 2);
        }

        void loadCameraInformation(glm::vec3 position, Config* config) {
            Shader::setVec3("cameraPosition", position);
            Shader::setVec2("nearFarPlane", glm::vec2(config->near_plane, config->far_plane));
        }

        void loadHeight(float height) {
            Shader::setFloat("height", height);
        }

        void loadWaveTime(float time) {
            Shader::setFloat("waveTime", time);
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

        void loadSpecularLighting(float reflectance, float factor) {
            Shader::setFloat("specularReflectance", reflectance);
            Shader::setFloat("specularFactor", factor);
        }

        void loadLightDirection(glm::vec3 direction) {
            Shader::setVec3("lightDirection", direction);
        }

        void loadFresnel(float fresnel) {
            Shader::setFloat("fresnelReflectiveness", fresnel);
        }

        void loadWaterColour(glm::vec3 colour) {
            Shader::setVec3("waterColour", colour);
        }

    private:
        void bindAttributes() {
            Shader::bindAttribute(0, "in_position");
            Shader::bindAttribute(4, "in_indicators");
        }
};
#endif //ITU_GRAPHICS_PROGRAMMING_WATER_SHADER_H
