//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_MODEL_H
#define ITU_GRAPHICS_PROGRAMMING_MODEL_H

struct Model {
    private:
        int vao;
        int vertexCount;

    public:
        Model(int vao, int vertexCount) {
            this->vao = vao;
            this->vertexCount = vertexCount;
        }

        int getVao() {
            return vao;
        }

        int getVertexCount() {
            return vertexCount;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_MODEL_H
