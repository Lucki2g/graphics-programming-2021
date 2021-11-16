//
// Created by Lucki on 16-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_WATER_FBOS_H
#define ITU_GRAPHICS_PROGRAMMING_WATER_FBOS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WaterFBOs {

    private:
        Config* config;
        unsigned int reflectionFrameBuffer;
        unsigned int reflectionTexture;
        unsigned int reflectionDepthBuffer;

        unsigned int refractionFrameBuffer;
        unsigned int refractionTexture;
        unsigned int refractionDepthTexture;

        void initReflection() {
            reflectionFrameBuffer = createFrameBuffer();
            reflectionTexture = createTextureBufferA(config->reflection_width, config->reflection_height);
            reflectionDepthBuffer = createDepthBuffer(config->reflection_width, config->reflection_height);
            unbindFrameBuffer();
        }

        void initRefraction() {
            refractionFrameBuffer = createFrameBuffer();
            refractionTexture = createTextureBufferA(config->refraction_width, config->refraction_height);
            refractionDepthTexture = createDepthBufferA(config->refraction_width, config->refraction_height);
            unbindFrameBuffer();
        }

        void bindFrameBuffer(int id, int w, int h) {
            glBindTexture(GL_TEXTURE_2D, 0); // make sure texture is not bound
            glBindFramebuffer(GL_FRAMEBUFFER, id); // bind framebuffer
            glViewport(0, 0, w, h); // set resolution
        }

        int createFrameBuffer() {
            unsigned int frameBuffer;
            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
            glDrawBuffer(GL_COLOR_ATTACHMENT0); // what color buffer do we render to?
            return frameBuffer;
        }

        /** BUFFER ATTACHMENTS **/
        int createTextureBufferA(int w, int h) {
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0); // add texture to currently bound fbo
            std::cout << "texture ID: " << texture << std::endl;
            return texture;
        }

        int createDepthBufferA(int w, int h) {
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); // 32 bit depth texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0); // add texture to currently bound fbo
            return texture;
        }

        /** RENDER BUFFERS **/
        int createDepthBuffer(int w, int h) {
            unsigned int buffer;
            glGenRenderbuffers(1, &buffer);
            glBindRenderbuffer(GL_RENDERBUFFER, buffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer);
            return buffer;
        }

    public:
        WaterFBOs(Config* config) {
            this->config = config;
            initReflection();
            initRefraction();
        }

        void unbindFrameBuffer() { // unbinds by switching to default buffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, config->width, config->height);
        }

        void bindReflection() {
            bindFrameBuffer(reflectionFrameBuffer, config->reflection_width, config->reflection_height);
        }

        void bindRefraction() {
            bindFrameBuffer(refractionFrameBuffer, config->refraction_width, config->refraction_height);
        }

        int getReflectionTex() {
            return reflectionTexture;
        }

        int getRefractionTex() {
            return refractionTexture;
        }

        int getRefractionDepth() {
            return refractionDepthTexture;
        }

        void clean() {
            glDeleteFramebuffers(1, &reflectionFrameBuffer);
            glDeleteFramebuffers(1, &refractionFrameBuffer);

            glDeleteTextures(1, &reflectionTexture);
            glDeleteTextures(1, &refractionTexture);
            glDeleteTextures(1, &refractionDepthTexture);

            glDeleteRenderbuffers(1, &reflectionDepthBuffer);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_WATER_FBOS_H
