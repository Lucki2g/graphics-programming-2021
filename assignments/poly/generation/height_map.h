//
// Created by Lucki on 13-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_HEIGHT_MAP_H
#define ITU_GRAPHICS_PROGRAMMING_HEIGHT_MAP_H

#define STB_IMAGE_IMPLEMENTATION
#include "../util/stb_image.h"
#include "../config.h"

class HeightMap {

    private:
        Config* config;
        int size;
        std::vector<std::vector<float>> noiseData = {};

        // https://www.cplusplus.com/forum/beginner/267364/
        void readFile(const char* path) {
            int n = 3;
            unsigned char *data = stbi_load(path, &size, &size, &n, 3);

            if (!data) {
                std::cout << "unable to load file: " << path << std::endl;
                return;
            }

            for (int i = 0; i < size + 1; i++)
                noiseData.push_back({});

            float old_range = (256 - 0);
            float new_range = (256 - -256);

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    unsigned char r = data[(i + size * j) * 3];
                    unsigned char g = data[(i + size * j) * 3 + 1];
                    unsigned char b = data[(i + size * j) * 3 + 2];
                    // unsigned char a = n >= 4 ? pixelOffset[3] : 0xff;
                    // std::cout << j << ":" << r << ":" << i << std::endl;

                    float height = (((((int) r) * new_range) / old_range) - 256.0f) / 256.0f;
                    noiseData.at(j).push_back(height); // change range from [0:255] to [-1:1]
                }
            }
            stbi_image_free(data);
        }

    public:
        HeightMap(const char* path, Config* config) {
            size = config->terrain_size;
            this->config = config;
            readFile(path);
        }

        float getHeight(int x, int z) {
            return noiseData.at(x).at(z) * config->amplitude;
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_HEIGHT_MAP_H
