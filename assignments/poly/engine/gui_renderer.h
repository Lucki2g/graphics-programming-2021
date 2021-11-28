//
// Created by Lucki on 16-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_GUI_RENDERER_H
#define ITU_GRAPHICS_PROGRAMMING_GUI_RENDERER_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shaders/gui/gui_shader.h>
#include "model_loader.h"
#include "../util/guis/gui_texture.h"

class GuiRenderer {
    private:
        Model* quad;
        GuiShader* shader;

    public:
        GuiRenderer(Loader* loader){
            std::vector<float> positions = {
                    -1.0f, 1.0f,
                    -1.0f, -1.0f,
                    1.0f, 1.0f,
                    1.0f, -1.0f
            };
            this->quad = loader->loadToVao(positions);
            this->shader = new GuiShader();
        }

        void render(std::vector<GuiTexture*> guis) {
            shader->Shader::start();
            glBindVertexArray(quad->getVao());
            glEnableVertexAttribArray(0);
            for (GuiTexture* texture : guis) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture->getTexture());
                glm::mat4 translation = glm::translate(glm::vec3(texture->getPosition(), 0));
                glm::mat4 scale = glm::scale(glm::vec3(texture->getScale(), 1.0f));
                shader->loadTransformationMatrix(translation * scale);
                shader->loadIsDepth(texture->isDepthTexture());
                glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->getVertexCount());
            }
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
            shader->Shader::stop();
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_GUI_RENDERER_H
