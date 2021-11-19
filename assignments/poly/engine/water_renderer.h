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

        void bind() {
            Model* model = water->getModel();
            glBindVertexArray(model->getVao());
            glEnableVertexAttribArray(0); // positions
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, fbos->getReflectionTex()); // reflection fbo
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fbos->getRefractionTex()); // refraction fbo
        }

        void unbind() {
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        }

        void loadWater(WaterShader* shader) {
            glm::mat4 position = glm::translate(glm::vec3(water->getPosition()));
            glm::mat4 scale = glm::scale(glm::vec3(water->getScale()));
            shader->loadTransformationMatrix(position * scale);
            glDrawElements(GL_TRIANGLES, water->getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
        }

    public:
        WaterRenderer(glm::mat4 projectionMatrix) {
            shader = new WaterShader();
            shader->Shader::start();
            shader->loadTextures();
            shader->loadProjectionMatrix(projectionMatrix);
            shader->Shader::stop();
        }

        void render(glm::mat4 viewMatrix, glm::vec3 camPosition) {
            shader->Shader::start();
            shader->loadViewMatrix(viewMatrix);
            shader->loadCameraPosition(camPosition);
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
