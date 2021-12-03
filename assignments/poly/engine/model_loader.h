//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_MODEL_LOADER_H
#define ITU_GRAPHICS_PROGRAMMING_MODEL_LOADER_H

#include "model.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // colours
    glm::vec3 Colours;
};

class Loader {

    private:
        std::vector<GLuint> vaos;
        std::vector<GLuint> vbos;
        std::vector<GLuint> textures;

    public:
        Model* loadToVao(const std::vector<float> &positions) {
            int vao = createVao();
            storeDataInAttributeList(0, 2, positions);
            unbindVao();
            return new Model(vao, positions.size() / 2);
        }

        Model* loadToVao(const std::vector<float> positions, const std::vector<float> indicators) {
            int vao = createVao();
            storeDataInAttributeList(0, 2, positions);
            storeDataInAttributeList(4, 4, indicators);
            unbindVao();
            return new Model(vao, positions.size() / 2);
        }

        Model* loadToVao(const std::vector<float> &positions, const std::vector<unsigned int> indices) {
            int vao = createVao();
            bindIndicesBuffer(indices);
            storeDataInAttributeList(0, 2, positions);
            unbindVao();
            return new Model(vao, indices.size());
        }

        Model* loadToVao(const std::vector<float> &positions, const std::vector<float> normals, const std::vector<unsigned int> indices) {
            int vao = createVao();
            bindIndicesBuffer(indices);
            storeDataInAttributeList(0, 3,positions);
            storeDataInAttributeList(1, 3,normals);
            unbindVao();
            return new Model(vao, indices.size());
        }

        Model* loadToVaoNoNormals(const std::vector<float> &positions, const std::vector<float> colours, const std::vector<unsigned int> indices) {
            int vao = createVao();
            bindIndicesBuffer(indices);
            storeDataInAttributeList(0, 3,positions);
            storeDataInAttributeList(2, 3,colours);
            unbindVao();
            return new Model(vao, indices.size());
        }

        Model* loadToVao(const std::vector<float> &positions, const std::vector<float> normals, const std::vector<float> colours, const std::vector<unsigned int> indices) {
            int vao = createVao();

            bindIndicesBuffer(indices);
            storeDataInAttributeList(0, 3,positions);
            storeDataInAttributeList(1, 3,normals);
            storeDataInAttributeList(2, 3,colours);
            unbindVao();
            return new Model(vao, indices.size());
        }

        Model* loadToVao(const std::vector<float> &positions, const std::vector<float> normals, const std::vector<float> colours) {

            std::vector<Vertex> vertices;
            for(unsigned int i = 0; i < positions.size() / 3; i++)
            {
                Vertex vertex;
                glm::vec3 vector;
                // positions
                vertex.Position = glm::vec3(positions.at(i * 3), positions.at(3 * i + 1), positions.at(3 * i + 2));
                // normals
                vertex.Normal = glm::vec3(normals.at(i * 3), normals.at(3 * i + 1), normals.at(3 * i + 2));
                // colours
                vertex.Colours = glm::vec3(colours.at(i * 3), colours.at(3 * i + 1), colours.at(3 * i + 2));

                vertices.push_back(vertex);
            }


            int vao = createVao();
            unsigned int VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colours));
            unbindVao();
            return new Model(vao, vertices.size());
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

        void storeDataInAttributeList(int attributeNumber, int size, const std::vector<float> &data, bool normalized = false) {
            unsigned int VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

            // fill data
            glVertexAttribPointer(attributeNumber, size, GL_FLOAT, normalized ? GL_TRUE : GL_FALSE, 0, 0);

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
