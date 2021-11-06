//
// Created by Lucki on 06-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_CAMERA_H
#define ITU_GRAPHICS_PROGRAMMING_CAMERA_H

#include "../glmutils.h"

class Camera {
    private:
        glm::vec3 position = glm::vec3(0);
        glm::vec3 forward = glm::vec3(0,0,-1);
        float pitch, yaw = -90.0f;

    public:
        glm::vec3 getPosition() {
            return position;
        }

        glm::vec3 getForward() {
            return forward;
        }

        float getPitch() {
            return pitch;
        }

        float getYaw() {
            return yaw;
        }

        void setForward(glm::vec3 forward) {
            this->forward = forward;
        }

        void setPitch(float pitch) {
            this->pitch = pitch;
        }

        void setYaw(float yaw) {
            this->yaw = yaw;
        }

        void move(glm::vec3 movement) {
            position += movement;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_CAMERA_H
