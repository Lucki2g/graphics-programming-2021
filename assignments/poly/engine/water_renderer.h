//
// Created by Lucki on 16-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_WATER_RENDERER_H
#define ITU_GRAPHICS_PROGRAMMING_WATER_RENDERER_H

#include <shaders/water/water_shader.h>
#include <shaders/water/water_fbos.h>

class WaterRenderer {
    private:
        Entity* water;
        WaterFBOs* fbos;
        WaterShader* shader;
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
            shader = new WaterShader();
            shader->Shader::start();
            shader->loadTextures();
            shader->loadProjectionMatrix(projectionMatrix);
            shader->loadHeight(config->water_height);
            shader->Shader::stop();
        }

        void render(Light* sun, glm::mat4 viewMatrix, glm::vec3 camPosition, Config* config) {
            shader->Shader::start();
            shader->loadLight(sun);
            shader->loadViewMatrix(viewMatrix);
            shader->loadAmbientLighting(config->ambientLightColour, config->ambientLightIntensity, config->ambientReflectance);
            shader->loadDiffuseLighting(config->diffuseReflectance);
            shader->loadCameraInformation(camPosition, config);
            shader->loadWaveTime(time += config->wave_speed);
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
