//
// Created by Lucki on 05-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_TERRAIN_H
#define ITU_GRAPHICS_PROGRAMMING_TERRAIN_H

class Terrain {
    private const float SIZE = 800;
    private const int VERTEX_COUNT = 128;

    private int Vao;
    private float x, z;

    public Terrain(int vao, int gridX, int gridZ) {
        this.Vao = vao;
        this.x = gridX * SIZE;
        this.z = gridZ = SIZE;
    }

    public generateTerrain() {

    }
};

#endif //ITU_GRAPHICS_PROGRAMMING_TERRAIN_H
