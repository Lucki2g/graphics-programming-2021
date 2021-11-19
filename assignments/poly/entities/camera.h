//
// Created by Lucki on 06-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_CAMERA_H
#define ITU_GRAPHICS_PROGRAMMING_CAMERA_H

#include "util/glmutils.h"

class Camera {
    private:
        glm::vec3 position;
        glm::vec3 forward = glm::vec3(0,0.5f,-1.0f);
        float pitch, yaw = -90.0f;

    public:
        Camera(glm::vec3 start) {
            this->position = start;
        }

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

        void invertPitch() {
            this->pitch = -pitch;
        }

        void invertYaw() {
            this->yaw = -yaw;
        }

        void calculateForward() {
            glm::vec3 front = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                                        sin(glm::radians(pitch)),
                                        sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
            forward = glm::normalize(front);
        }

        void calculate(double x, double y, glm::vec2 previous) {
            calculateForward();

            float xOffset = x - previous.x;
            float yOffset = previous.y - y; // reversed since y-coordinates range from bottom to top

            const float sensitivity = 0.1f;
            xOffset *= sensitivity;
            yOffset *= sensitivity;

            pitch = std::max(std::min(pitch + yOffset, 89.0f), -89.0f);
            yaw = (yaw + xOffset);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_CAMERA_H
