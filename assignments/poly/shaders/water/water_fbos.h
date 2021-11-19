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
            reflectionTexture = createColourAttachment(config->reflection_width, config->reflection_height);
            reflectionDepthBuffer = createDepthAttachment(config->reflection_width, config->reflection_height);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
                std::cout << "generated reflection: " << reflectionFrameBuffer << std::endl;
            unbindFrameBuffer();
        }

        void initRefraction() {
            refractionFrameBuffer = createFrameBuffer();
            refractionTexture = createColourAttachment(config->refraction_width, config->refraction_height);
            refractionDepthTexture = createRenderBuffer(config->refraction_width, config->refraction_height);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
                std::cout << "generated refraction: " << refractionFrameBuffer << std::endl;
            unbindFrameBuffer();
        }

        void bindFrameBuffer(int id, int w, int h) {
            glBindFramebuffer(GL_FRAMEBUFFER, id); // bind framebuffer
            glViewport(0, 0, w, h); // set resolution
        }

        unsigned int createFrameBuffer() {
            unsigned int frameBuffer;
            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
            // glDrawBuffer(GL_COLOR_ATTACHMENT0); // what color buffer do we render to?
            return frameBuffer;
        }

        /** BUFFER ATTACHMENTS **/
        unsigned int createColourAttachment(int w, int h) {
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0); // add texture to currently bound fbo
            return texture;
        }

        unsigned int createDepthAttachment(int w, int h) {
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); // 32 bit depth texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0); // add texture to currently bound fbo
            return texture;
        }

        /** RENDER BUFFERS **/
        unsigned int createRenderBuffer(int w, int h) {
            unsigned int rbo;
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
            return rbo;
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

        unsigned int getReflectionTex() {
            return reflectionTexture;
        }

        unsigned int getRefractionTex() {
            return refractionTexture;
        }

        unsigned int getRefractionDepth() {
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
