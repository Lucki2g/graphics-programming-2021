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
        int size;
        std::vector<int> heights = {};

        // https://www.cplusplus.com/forum/beginner/267364/
        void readFile(const char* path) {
            int n = 3;
            unsigned char *data = stbi_load(path, &size, &size, &n, 0);

            if (!data) {
                std::cout << "unable to load file: " << path << std::endl;
                return;
            }

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    unsigned bytePerPixel = n;
                    unsigned char* pixelOffset = data + (i + size * j) * bytePerPixel;
                    unsigned char r = pixelOffset[0];
                    unsigned char g = pixelOffset[1];
                    unsigned char b = pixelOffset[2];
                    unsigned char a = n >= 4 ? pixelOffset[3] : 0xff;
                    heights.push_back((int)r);
                }
            }
            stbi_image_free(data);
        }

    public:
        HeightMap(const char* path, Config* config) {
            size = 256;
            readFile(path);
        }

        float getHeight(int x, int z) {
            return heights.at(x + z * (size - 1));
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_HEIGHT_MAP_H
