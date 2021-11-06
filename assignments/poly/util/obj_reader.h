//
// Created by Lucki on 06-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_OBJ_READER_H
#define ITU_GRAPHICS_PROGRAMMING_OBJ_READER_H

#include "../engine/model.h"
#include "../engine/model_loader.h"
#include "OBJ_loader.h"

// https://github.com/Bly7/OBJ-Loader/blob/master/examples/1%20-%20LoadAndPrint/e1_loadandprint.cpp
class ObjReader {
    public:
        Model* loadObjModel(std::string filePath, Loader* loader) {
            objl::Loader objloader;
            bool loadout = objloader.LoadFile(filePath);
            if (!loadout) {
                std::cout << "Could not read file: " << filePath << std::endl;
                return NULL;
            }

            for (int i = 0; i < objloader.LoadedMeshes.size(); i++)
                objl::Mesh curMesh = objloader.LoadedMeshes[i];

            std::vector<float> positions;
            std::vector<float> normals;
            std::vector<unsigned int> indices;

            for (int i = 0; i < objloader.LoadedMeshes[0].Vertices.size(); i++) {
                objl::Vertex vertex = objloader.LoadedMeshes[0].Vertices[i];
                positions.push_back(vertex.Position.X);
                positions.push_back(vertex.Position.Y);
                positions.push_back(vertex.Position.Z);
                normals.push_back(vertex.Normal.X);
                normals.push_back(vertex.Normal.Y);
                normals.push_back(vertex.Normal.Z);
            }

            for (int i = 0; i < objloader.LoadedMeshes[0].Indices.size(); i++) {
                indices.push_back(objloader.LoadedMeshes[0].Indices[i]);
            }

            return loader->loadToVao(positions, normals, indices);
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_OBJ_READER_H
