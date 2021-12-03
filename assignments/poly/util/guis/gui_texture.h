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
        bool isDepth;

    public:
        GuiTexture(int texture, glm::vec2 pos, glm::vec2 s, bool depth = false) {
            this->texture = texture;
            this->position = pos;
            this->scale = s;
            this->isDepth = depth;
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

        bool isDepthTexture() {
            return isDepth;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_GUI_TEXTURE_H
