//
// Created by Lucki on 20-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_MESH_GENERATOR_H
#define ITU_GRAPHICS_PROGRAMMING_MESH_GENERATOR_H

class MeshGenerator {
    private:
        Config* config;
        ColourGenerator* colourGenerator;

    public:

        MeshGenerator(Config* config, ColourGenerator* colourGenerator) {
            this->config = config;
            this->colourGenerator = colourGenerator;
        }

        void store(glm::vec3 pos, glm::vec3 normal, glm::vec3 colour,
                   std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
            positions.push_back(pos.x);
            positions.push_back(pos.y);
            positions.push_back(pos.z);

            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);

            colours.push_back(colour.x);
            colours.push_back(colour.y);
            colours.push_back(colour.z);
        }

        void store(glm::vec2 pos, glm::vec4 ind, std::vector<float> &positions, std::vector<float> &indicators) {
            positions.push_back(pos.x);
            positions.push_back(pos.y);

            indicators.push_back(ind.x);
            indicators.push_back(ind.y);
            indicators.push_back(ind.z);
            indicators.push_back(ind.w);
        }

        void storeTriangle(std::vector<glm::vec3> corners, std::vector<glm::vec3> cols, glm::vec3 normal, int i0, int i1, int i2,
                           std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
            store(corners.at(i0), normal, cols.at(i0), positions, normals, colours);
            store(corners.at(i1), normal, cols.at(i1), positions, normals, colours);
            store(corners.at(i2), normal, cols.at(i2), positions, normals, colours);
        }

        void storeTriangle(std::vector<glm::vec2> corners, int i0, int i1, int i2,
                           std::vector<float> &positions, std::vector<float> &indicators) {

            glm::vec2 v = corners.at(i0);
            glm::vec2 v1 = corners.at(i1);
            glm::vec2 v2 = corners.at(i2);

            store(v, getIndicator(v, v1, v2), positions, indicators);
            //test(v, getIndicator(v, v1, v2));
            store(v1, getIndicator(v1, v2, v), positions, indicators);
            store(v2, getIndicator(v2, v, v1), positions, indicators);
        }

        /*float createOffset(float x, float z) {
            float radiansX = (x / 10.0f) * 2.0f * PI;
            float radiansZ = (z / 10.0f) * 2.0f * PI;
            return 1.0f * 0.5f * (sin(radiansZ) + cos(radiansX));
        }

        glm::vec3 distort(glm::vec3 vertex) {
            float distX = createOffset(vertex.x, vertex.z);
            float distY = createOffset(vertex.x, vertex.z);
            float distZ = createOffset(vertex.x, vertex.z);
            return vertex + glm::vec3(distX, distY, distZ);
        }

        void test(glm::vec2 v1, glm::vec4 ind) {
            glm::vec3 v = glm::vec3(v1.x, 0.0f, v1.y);
            glm::vec3 v2 = v + glm::vec3(ind.x, 0.0f, ind.y);
            glm::vec3 v3 = v + glm::vec3(ind.z, 0.0f, ind.w);

            v = distort(v);
            v2 = distort(v2);
            v3 = distort(v3);

            glm::vec3 t1 = v2 - v;
            glm::vec3 t2 = v3 - v;

            glm::vec3 res = glm::normalize(glm::cross(t1, t2));
            std::cout << res.x << ", " << res.y << ", " << res.z << std::endl;
        }*/

        glm::vec4 getIndicator(glm::vec2 v, glm::vec2 v1, glm::vec2 v2) {
            glm::vec2 offset1 = v1 - v;
            glm::vec2 offset2 = v2 - v;
            return glm::vec4(offset1, offset2); // could be saved as bytes to save space
            // std::cout << indicator.x << ", " << indicator.y << ", " << indicator.z << ", " << indicator.w << std::endl;
        }

        Model* quad(Loader* loader) {
            std::vector<float> positions = {
                    0.5f,  0.5f, 0.0f,  // top right
                    0.5f, -0.5f, 0.0f,  // bottom right
                    -0.5f, -0.5f, 0.0f,  // bottom left
                    -0.5f,  0.5f, 0.0f   // top left
            };

            std::vector<float> normals {
                    0, 1, 0,
                    0, 1, 0,
                    0, 1, 0,
                    0, 1, 0
            };

            std::vector<unsigned int> indices {
                    0, 1, 2,
                    1, 2, 3
            };

            return loader->loadToVao(positions, normals, indices);
        }

        /** NORMAL, MESH, FLAT **/
        Model* proceduralTerrain(Loader* loader, bool specialIndices) {
            HeightsGenerator* generator = new HeightsGenerator(config);

            float** heights = generateHeights(generator);
            glm::vec3** colours = generateColours(heights);
            glm::vec3** normals = generateNormals(heights);

            std::vector<float> out_positions;
            std::vector<float> out_normals;
            std::vector<float> out_colours;
            std::vector<unsigned int> out_indices;

            for (int z = 0; z < config->terrain_size; z++) {
                for (int x = 0; x < config->terrain_size; x++) {
                    float height = heights[z][x];
                    /*glm::vec3 position = glm::vec3(  -((float) x/((float) config->terrain_size - 1)) * config->size,
                                                     height,
                                                     -((float) z/((float) config->terrain_size - 1)) * config->size
                    );*/

                    glm::vec3 position = glm::vec3(z, height, x);
                    out_positions.push_back(position.x);
                    out_positions.push_back(position.y);
                    out_positions.push_back(position.z);

                    glm::vec3 normal = normals[z][x];
                    out_normals.push_back(normal.x);
                    out_normals.push_back(normal.y);
                    out_normals.push_back(normal.z);

                    glm::vec3 colour = glm::normalize(colours[z][x]);
                    out_colours.push_back(colour.x);
                    out_colours.push_back(colour.y);
                    out_colours.push_back(colour.z);
                    //std::cout << colour.x << "\t" << colour.y << "\t" << colour.z << std::endl;
                }
            }

            if (specialIndices) generateSpecialIndices(out_indices);
            else generateIndices(out_indices);

            return loader->loadToVao(out_positions, out_normals, out_colours, out_indices);
        }

        /** VERTEX_DUB **/
        Model* generateDubVertexTerrain(Loader* loader) {
            HeightsGenerator* generator = new HeightsGenerator(config);

            float** gen_heights = generateHeights(generator);

            std::vector<float> out_positions;
            std::vector<float> out_normals;
            std::vector<float> out_colours;

            for (int x = 0; x < config->terrain_size; x++) {
                for (int z = 0; z < config->terrain_size; z++) {

                    std::vector<float> heights = {
                            getHeight(z, x, gen_heights),
                            getHeight(z + 1, x, gen_heights),
                            getHeight(z, x + 1, gen_heights),
                            getHeight(z + 1, x + 1, gen_heights),
                    };

                    std::vector<glm::vec3> corners = {
                            glm::vec3(z, heights.at(0), x),
                            glm::vec3(z + 1, heights.at(1), x),
                            glm::vec3(z, heights.at(2), x + 1),
                            glm::vec3(z + 1, heights.at(3), x + 1)
                    };

                    std::vector<glm::vec3> cols = {
                            glm::normalize(colourGenerator->getColour(heights.at(0), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(1), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(2), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(3), config->amplitude))
                    };

                    glm::vec3 TLNormal = getNormal(corners.at(0), corners.at(1), corners.at(2));
                    glm::vec3 BRNormal = getNormal(corners.at(2), corners.at(1), corners.at(3));

                    storeTriangle(corners, cols, TLNormal, 0, 1, 2, out_positions, out_normals, out_colours);
                    storeTriangle(corners, cols, BRNormal, 2, 1, 3, out_positions, out_normals, out_colours);
                }
            }

            return loader->loadToVao(out_positions, out_normals, out_colours);
        }

        /** GEOMETRY **/
        Model* generateGeoTerrain(Loader* loader) {
            HeightsGenerator* generator = new HeightsGenerator(config);

            float** heights = generateHeights(generator);
            glm::vec3** colours = generateColours(heights);

            std::vector<float> out_positions;
            std::vector<float> out_colours;
            std::vector<unsigned int> out_indices;

            for (int z = 0; z < config->terrain_size; z++) {
                for (int x = 0; x < config->terrain_size; x++) {

                    float height = heights[z][x];
                    glm::vec3 position = glm::vec3(z, height, x);
                    out_positions.push_back(position.x);
                    out_positions.push_back(position.y);
                    out_positions.push_back(position.z);

                    glm::vec3 colour = glm::normalize(colours[z][x]);
                    out_colours.push_back(colour.x);
                    out_colours.push_back(colour.y);
                    out_colours.push_back(colour.z);
                }
            }

            generateIndices(out_indices);

            return loader->loadToVaoNoNormals(out_positions, out_colours, out_indices);
        }

        /** EQUILINOX **/
        Model* generateEquilinoxTerrain(Loader* loader) {
            HeightsGenerator* generator = new HeightsGenerator(config);

            float** gen_heights = generateHeights(generator);

            std::vector<float> out_positions;
            std::vector<float> out_colours;
            std::vector<float> out_normals;
            std::vector<unsigned int> out_indices;

            unsigned int lastLine = config->terrain_size - 2;


            std::vector<TerrainQuad*> lastRow;

            for (int z = 0; z < config->terrain_size - 1; z++) {
                for (int x = 0; x < config->terrain_size - 1; x++) {
                    std::vector<float> heights = {
                            getHeight(z, x, gen_heights),
                            getHeight(z + 1, x, gen_heights),
                            getHeight(z, x + 1, gen_heights),
                            getHeight(z + 1, x + 1, gen_heights),
                    };

                    std::vector<glm::vec3> corners = {
                            glm::vec3(z, heights.at(0), x),
                            glm::vec3(z + 1, heights.at(1), x),
                            glm::vec3(z, heights.at(2), x + 1),
                            glm::vec3(z + 1, heights.at(3), x + 1)
                    };

                    std::vector<glm::vec3> cols = {
                            glm::normalize(colourGenerator->getColour(heights.at(0), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(1), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(2), config->amplitude)),
                            glm::normalize(colourGenerator->getColour(heights.at(3), config->amplitude))
                    };

                    bool flipped = x % 2 != z % 2;
                    glm::vec3 TLNormal = getNormal(corners.at(0), corners.at(1), corners.at(flipped ? 3 : 2));
                    glm::vec3 BRNormal = getNormal(corners.at(2), corners.at(flipped ? 0 : 1), corners.at(3));

                    TerrainQuad* quad = new TerrainQuad(z, x, lastLine, corners, cols, TLNormal, BRNormal);
                    quad->storeData(out_positions, out_normals, out_colours);
                    if (z == lastLine) lastRow.push_back(quad);
                }
            }

            // last row
            for (TerrainQuad* g : lastRow) g->storeLastRowData(out_positions, out_normals, out_colours);

            generateEquilinoxIndices(out_indices);
            return loader->loadToVao(out_positions, out_normals, out_colours, out_indices);
        }

        /** WATER **/
        Model* generateWater(Loader* loader) {

            std::vector<float> out_positions;
            std::vector<float> out_indicators;

            for (int z = 0; z < config->terrain_size; z++) {
                for (int x = 0; x < config->terrain_size; x++) {

                    std::vector<glm::vec2> corners = {
                            glm::vec2(x, z),
                            glm::vec2(x + 1, z),
                            glm::vec2(x, z + 1),
                            glm::vec2(x + 1, z + 1)
                    };

                    storeTriangle(corners, 0, 1, 2, out_positions, out_indicators);
                    storeTriangle(corners, 2, 1, 3, out_positions, out_indicators);
                }
            }

            return loader->loadToVao(out_positions, out_indicators);//out_normals);
        }

        /************** INDICES ***************/
        void generateIndices(std::vector<unsigned int> &indices) {
            int size = config->terrain_size;
            //std::cout << "INDICES: " << std::endl;
            for (int col = 0; col < size - 1; col++) {
                for (int row = 0; row < size - 1; row++) {
                    unsigned int topLeft = (row * size) + col;
                    unsigned int topRight = topLeft + 1;
                    unsigned int bottomLeft = ((row + 1) * size) + col;
                    unsigned int bottomRight = bottomLeft + 1;
                    indices.push_back(topLeft);
                    indices.push_back(bottomLeft);
                    indices.push_back(bottomRight);
                    //std::cout << "T1: " << topLeft << " " << bottomLeft << " " << bottomRight << std::endl;
                    indices.push_back(topLeft);
                    indices.push_back(bottomRight);
                    indices.push_back(topRight);
                    //std::cout << "T2: " << topLeft << " " << bottomRight << " " << topRight << std::endl;
                }
            }
        }

        void generateSpecialIndices(std::vector<unsigned int> &indices) {
            int size = config->terrain_size;
            for (int col = 0; col < size - 1; col++) {
                for (int row = 0; row < size - 1; row++) {
                    unsigned int topLeft = (row * size) + col;
                    unsigned int topRight = topLeft + 1;
                    unsigned int bottomLeft = ((row + 1) * size) + col;
                    unsigned int bottomRight = bottomLeft + 1;
                    if (row % 2 == 0) {
                        indices.push_back(topLeft);
                        indices.push_back(bottomLeft);
                        if (col % 2 == 0)
                            indices.push_back(topRight);
                        else
                            indices.push_back(bottomRight);
                        indices.push_back(bottomRight);
                        indices.push_back(topRight);
                        if (col % 2 == 0)
                            indices.push_back(bottomLeft);
                        else
                            indices.push_back(topLeft);
                    } else {
                        indices.push_back(topRight);
                        indices.push_back(topLeft);
                        if (col % 2 == 0)
                            indices.push_back(bottomRight);
                        else
                            indices.push_back(bottomLeft);
                        indices.push_back(bottomLeft);
                        indices.push_back(bottomRight);
                        if (col % 2 == 0)
                            indices.push_back(topLeft);
                        else
                            indices.push_back(topRight);
                    }
                }
            }
        }

        void generateEquilinoxIndices(std::vector<unsigned int> &indices) {
            int size = config->terrain_size - 1;
            int rowCount = size * 2;
            // top rows
            for (int row = 0; row < size - 2; row++) { // -2 for last lines
                for (int col = 0; col < size; col++) {
                    unsigned int topLeft = (row * rowCount) + (col * 2);
                    unsigned int topRight = topLeft + 1;
                    unsigned int bottomLeft = topLeft + rowCount;
                    unsigned int bottomRight = bottomLeft + 1;

                    bool flipped = row % 2 != col % 2;
                    indices.push_back(topLeft);
                    indices.push_back(bottomLeft);
                    indices.push_back(flipped ? bottomRight : topRight);

                    indices.push_back(topRight);
                    indices.push_back(flipped ? topLeft : bottomLeft);
                    indices.push_back(bottomRight);
                }
            }
            // second last row
            int row = size - 2;
            for (int col = 0; col < size; col++) {
                unsigned int topLeft = (row * rowCount) + (col * 2);
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = topLeft + rowCount - col;
                unsigned int bottomRight = bottomLeft + 1;

                bool flipped = col % 2 != col % 2;
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(flipped ? bottomRight : topRight);

                indices.push_back(topRight);
                indices.push_back(flipped ? topLeft : bottomLeft);
                indices.push_back(bottomRight);
            }
            // last row
            row++;
            for (int col = 0; col < size; col++) {
                unsigned int topLeft = (row * rowCount) + col;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = topLeft + config->terrain_size;
                unsigned int bottomRight = bottomLeft + 1;

                bool flipped = col % 2 != col % 2;
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(flipped ? bottomRight : topRight);

                indices.push_back(topRight);
                indices.push_back(flipped ? topLeft : bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        /************** HEIGHT ***************/
        float** generateHeights(HeightsGenerator* generator) {
            float** heights = new float*[config->terrain_size + 1];
            for (int z = 0; z < config->terrain_size + 1; z++) {
                heights[z] = new float[config->terrain_size + 1];
                for (int x = 0; x < config->terrain_size + 1; x++) {
                    float height = generator->generateHeight(x, z, config->HEIGHTMAP);
                    //std::cout << "x: " << x << " z: " << z << " = " << height << std::endl;
                    heights[z][x] = height;
                }
            }
            return heights;
        }

        /************** COLOUR ***************/
        glm::vec3** generateColours(float** heights) {
            glm::vec3** colours = new glm::vec3* [config->terrain_size + 1];
            //std::cout << "COLOURS: " << std::endl;
            for (int z = 0; z < config->terrain_size + 1; z++) {
                colours[z] = new glm::vec3 [config->terrain_size + 1];
                for (int x = 0; x < config->terrain_size + 1; x++) {
                    float height = heights[z][x];
                    glm::vec3 colour = colourGenerator->getColour(height, config->amplitude);
                    //std::cout << "height:" << height << " = " << colour.x << "," << colour.y << "," << colour.z << std::endl;
                    colours[z][x] = colour;
                }
            }
            return colours;
        }

        /************** NORMAL ***************/
        glm::vec3** generateNormals(float** heights) {
            glm::vec3** normals = new glm::vec3* [config->terrain_size + 1];
            //std::cout << "NORMALS: " << std::endl;
            for (int z = 0; z < config->terrain_size + 1; z++) {
                normals[z] = new glm::vec3 [config->terrain_size + 1];
                for (int x = 0; x < config->terrain_size + 1; x++) {
                    glm::vec3 normal = getNormal(x, z, heights);
                    //std::cout << " = " << normal.x << "," << normal.y << "," << normal.z << std::endl;
                    normals[z][x] = normal;
                }
            }
            return normals;
        }

        glm::vec3 getNormal(int x, int z, float** heights) {
            float HL = getHeight(x - 1, z, heights);
            float HR = getHeight(x + 1, z, heights);
            float HD = getHeight(x, z - 1, heights);
            float HU = getHeight(x, z + 1, heights);
            glm::vec3 normal = glm::vec3(HL - HR, 2.0f, HD - HU);
            glm::normalize(normal);
            return normal;
        }

        float getHeight(int x, int z, float** heights) {
            int max = config->terrain_size - 1;
            x = std::min(std::max(0, x), max);
            z = std::min(std::max(0, z), max);
            return heights[z][x];
        }

        glm::vec3 getNormal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
            glm::vec3 tangentA = v1 - v0;
            glm::vec3 tangentB = v2 - v0;
            return glm::normalize(glm::cross(tangentA, tangentB));
        }

        /** QUAD OF MESH**/
        struct TerrainQuad {
            int row, col, lastrow;
            std::vector<glm::vec3> corners;
            std::vector<glm::vec3> cols;
            glm::vec3 left, right;

            TerrainQuad(int row, int col, int lastrow, std::vector<glm::vec3> corners, std::vector<glm::vec3> cols, glm::vec3 T, glm::vec3 B) {
                this->row = row;
                this->col = col;
                this->lastrow = lastrow;
                this->corners = corners;
                this->cols = cols;
                this->left = -T;
                this->right = -B;
            }

            void storeData(std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
                storeTopLeft(positions, normals, colours);
                if (row != lastrow || col == lastrow) storeTopRight(positions, normals, colours);
            }

            void storeLastRowData(std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
                if (col == 0) storeBottomLeft(positions, normals, colours);
                storeBottomRight(positions, normals, colours);
            }

            void storeTopLeft(std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
                positions.push_back(corners.at(0).x);
                positions.push_back(corners.at(0).y);
                positions.push_back(corners.at(0).z);

                normals.push_back(left.x);
                normals.push_back(left.y);
                normals.push_back(left.z);

                colours.push_back(cols.at(0).x);
                colours.push_back(cols.at(0).y);
                colours.push_back(cols.at(0).z);
            }

            void storeTopRight(std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
                positions.push_back(corners.at(2).x);
                positions.push_back(corners.at(2).y);
                positions.push_back(corners.at(2).z);

                normals.push_back(right.x);
                normals.push_back(right.y);
                normals.push_back(right.z);

                colours.push_back(cols.at(2).x);
                colours.push_back(cols.at(2).y);
                colours.push_back(cols.at(2).z);
            }

            void storeBottomLeft(std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
                positions.push_back(corners.at(1).x);
                positions.push_back(corners.at(1).y);
                positions.push_back(corners.at(1).z);

                normals.push_back(left.x);
                normals.push_back(left.y);
                normals.push_back(left.z);

                colours.push_back(cols.at(1).x);
                colours.push_back(cols.at(1).y);
                colours.push_back(cols.at(1).z);
            }

            void storeBottomRight(std::vector<float> &positions, std::vector<float> &normals, std::vector<float> &colours) {
                positions.push_back(corners.at(3).x);
                positions.push_back(corners.at(3).y);
                positions.push_back(corners.at(3).z);

                normals.push_back(right.x);
                normals.push_back(right.y);
                normals.push_back(right.z);

                colours.push_back(cols.at(3).x);
                colours.push_back(cols.at(3).y);
                colours.push_back(cols.at(3).z);
            }
        };
};

#endif //ITU_GRAPHICS_PROGRAMMING_MESH_GENERATOR_H
