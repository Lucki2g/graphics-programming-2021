//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_MODEL_LOADER_H
#define ITU_GRAPHICS_PROGRAMMING_MODEL_LOADER_H

#include "model.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>

class Loader {

    private:
        std::vector<GLuint> vaos;
        std::vector<GLuint> vbos;

    public:
        Model* loadToVao(const std::vector<float> &positions, const std::vector<float> normals, const std::vector<unsigned int> indices) {
            int vao = createVao();
            bindIndicesBuffer(indices);
            storeDataInAttributeList(0, positions);
            storeDataInAttributeList(1, normals);
            unbindVao();
            return new Model(vao, indices.size());
        }

        Model* loadToVaoNoNormals(const std::vector<float> &positions, const std::vector<float> colours, const std::vector<unsigned int> indices) {
            int vao = createVao();
            bindIndicesBuffer(indices);
            storeDataInAttributeList(0, positions);
            storeDataInAttributeList(2, colours);
            unbindVao();
            return new Model(vao, indices.size());
        }

        Model* loadToVao(const std::vector<float> &positions, const std::vector<float> normals, const std::vector<float> colours, const std::vector<unsigned int> indices) {
            int vao = createVao();
            bindIndicesBuffer(indices);
            storeDataInAttributeList(0, positions);
            storeDataInAttributeList(1, normals);
            storeDataInAttributeList(2, colours);
            unbindVao();
            return new Model(vao, indices.size());
        }

        Model* loadToVao(const std::vector<float> &positions, const std::vector<float> normals, const std::vector<float> colours) {
            int vao = createVao();
            storeDataInAttributeList(0, positions);
            storeDataInAttributeList(1, normals);
            storeDataInAttributeList(2, colours);
            unbindVao();
            return new Model(vao, positions.size());
        }

        void clean() {
            glDeleteVertexArrays(vaos.size(), vaos.data());
            glDeleteBuffers(vbos.size(), vbos.data());
        }

    private:
        int createVao() {
            unsigned int VAO;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            vaos.push_back(VAO);
            return VAO;
        }

        void storeDataInAttributeList(int attributeNumber, const std::vector<float> &data) {
            unsigned int VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

            // fill data
            glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 0, 0);

            // unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            vbos.push_back(VBO);
        }

        void unbindVao() {
            glBindVertexArray(0);
        }

        void bindIndicesBuffer(const std::vector<unsigned int> &indices) {
            unsigned int EBO;
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            vbos.push_back(EBO);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_MODEL_LOADER_H
