//
// Created by Lucki on 16-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_WATER_RENDERER_H
#define ITU_GRAPHICS_PROGRAMMING_WATER_RENDERER_H

#include <shaders/water/water_shader.h>

class WaterRenderer {
    private:
        Entity* water;

        void bind() {
            Model* model = water->getModel();
            glBindVertexArray(model->getVao());
            glEnableVertexAttribArray(0); // positions
        }

        void unbind() {
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        }

        void loadWater(WaterShader* shader) {
            glm::mat4 position = glm::translate(glm::vec3(water->getPosition().x, 0, water->getPosition().z));
            shader->loadTransformationMatrix(position);
            glDrawElements(GL_TRIANGLES, water->getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
        }

    public:
        void render(WaterShader* shader) {
            bind();
            loadWater(shader);
            unbind();
        }

        void setWater(Entity* water) {
            this->water = water;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_WATER_RENDERER_H
