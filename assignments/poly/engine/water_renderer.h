//
// Created by Lucki on 16-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_WATER_RENDERER_H
#define ITU_GRAPHICS_PROGRAMMING_WATER_RENDERER_H

#include <shaders/water_shader.h>
#include <shaders/water/water_fbos.h>
#include <shaders/water/normal/normal_water_shader.h>
#include <map>
#include <shaders/water/lowpoly/lowpoly_water_shader.h>

class WaterRenderer {
    private:
        Entity* water;
        WaterFBOs* fbos;
        std::map<int, std::unique_ptr<WaterShader>> shaders;
        float time = 0.0f;

        void bind() {
            Model* model = water->getModel();
            glBindVertexArray(model->getVao());
            glEnableVertexAttribArray(0); // positions
            glEnableVertexAttribArray(4); // indicators
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, fbos->getReflectionTex()); // reflection fbo
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fbos->getRefractionTex()); // refraction fbo
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, fbos->getRefractionDepthTex()); // depth texture

            // enable alpha blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        void unbind() {
            glDisable(GL_BLEND);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(4);
            glBindVertexArray(0);
        }

        void loadWater(WaterShader* shader) {
            glDrawArrays(GL_TRIANGLES, 0, water->getModel()->getVertexCount());
        }

    public:
        WaterRenderer(glm::mat4 projectionMatrix, Config* config) {
            NormalWaterShader* normalShader;
            normalShader = new NormalWaterShader();
            normalShader->Shader::start();
            normalShader->loadTextures();
            normalShader->loadProjectionMatrix(projectionMatrix);
            normalShader->loadHeight(config->water_height);
            normalShader->Shader::stop();
            shaders.insert(std::pair<int, std::unique_ptr<WaterShader>>(NORMAL, normalShader));

            LowPolyWaterShader* lowpolyShader;
            lowpolyShader = new LowPolyWaterShader();
            lowpolyShader->Shader::start();
            lowpolyShader->loadTextures();
            lowpolyShader->loadProjectionMatrix(projectionMatrix);
            lowpolyShader->loadHeight(config->water_height);
            lowpolyShader->Shader::stop();
            shaders.insert(std::pair<int, std::unique_ptr<WaterShader>>(VERTEX_COPY, lowpolyShader));
            shaders.insert(std::pair<int, std::unique_ptr<WaterShader>>(MESH, lowpolyShader));
        }

        void render(Light* sun, glm::mat4 viewMatrix, glm::vec3 camPosition, Config* config) {

            int mode = config->generationSetting;

            if (shaders.find(mode) == shaders.end())
                return;

            WaterShader* shader = shaders[mode].get();

            shader->Shader::start();
            shader->loadLight(sun);
            shader->loadViewMatrix(viewMatrix);
            shader->loadAmbientLighting(config->ambientLightColour, config->ambientLightIntensity, config->ambientReflectance);
            shader->loadDiffuseLighting(config->diffuseReflectance);
            shader->loadSpecularLighting(config->specularReflectance, config->specularFactor);
            shader->loadLightDirection(config->lightDirection);
            shader->loadCameraInformation(camPosition, config);
            shader->loadWaveData(time += config->wave_speed, config->waveLength, config->waveAmplitude);
            shader->loadWaterColour(config->waterColour);
            shader->loadDistortion(config->distortionFunction);
            shader->loadFresnel(config->fresnelReflectiveness);
            bind();
            loadWater(shader);
            unbind();
            shader->Shader::stop();
        }

        void setWater(Entity* water, WaterFBOs* fbos) {
            this->water = water;
            this->fbos = fbos;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_WATER_RENDERER_H
