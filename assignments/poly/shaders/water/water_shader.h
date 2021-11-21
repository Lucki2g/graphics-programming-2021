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

    private:
        void bindAttributes() {
            Shader::bindAttribute(0, "in_position");
            Shader::bindAttribute(4, "in_indicators");
        }
};
#endif //ITU_GRAPHICS_PROGRAMMING_WATER_SHADER_H
