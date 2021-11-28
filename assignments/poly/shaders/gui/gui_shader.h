//
// Created by Lucki on 16-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_GUI_SHADER_H
#define ITU_GRAPHICS_PROGRAMMING_GUI_SHADER_H

#include "../shader_program.h"

const char* GUI_VERTEX_FILE = "shaders/gui/gui.vert";
const char* GUI_FRAGMENT_FILE = "shaders/gui/gui.frag";

class GuiShader : public Shader {
    public:
        GuiShader() : Shader(GUI_VERTEX_FILE, GUI_FRAGMENT_FILE) {
            bindAttributes();
        }

        void loadTransformationMatrix(glm::mat4 matrix) {
            Shader::setMat4("transformationMatrix", matrix);
        }

        void loadIsDepth(bool b) {
            Shader::setBool("isDepthTexture", b);
        }

    private:
        void bindAttributes() {
            Shader::bindAttribute(0, "in_position");
        }
};
#endif //ITU_GRAPHICS_PROGRAMMING_WATER_SHADER_H
