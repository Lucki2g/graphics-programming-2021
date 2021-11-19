//
// Created by Lucki on 16-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_GUI_TEXTURE_H
#define ITU_GRAPHICS_PROGRAMMING_GUI_TEXTURE_H

#include "../glmutils.h"

class GuiTexture {
    private:
        int texture;
        glm::vec2 position, scale;

    public:
        GuiTexture(int texture, glm::vec2 pos, glm::vec2 s) {
            this->texture = texture;
            this->position = pos;
            this->scale = s;
        }

        int getTexture() {
            return texture;
        }

        glm::vec2 getPosition() {
            return position;
        }

        glm::vec2 getScale() {
            return scale;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_GUI_TEXTURE_H
