// Primitives from exercise 5.2
std::vector<float> cubeVertices {-1.0f, -1.0f, 1.0f,
                                 1.0f, -1.0f, 1.0f,
                                 1.0f, 1.0f, 1.0f,
                                 -1.0f, 1.0f, 1.0f,
                                 -1.0f, -1.0f, -1.0f,
                                 1.0f, -1.0f, -1.0f,
                                 1.0f, 1.0f, -1.0f,
                                 -1.0f, 1.0f, -1.0f};
std::vector<unsigned int> cubeIndices {0, 1, 2,
                                       0, 2, 3,
                                       1, 5, 6,
                                       1, 6, 2,
                                       5, 4, 7,
                                       5, 7, 6,
                                       4, 0, 3,
                                       4, 3, 7,
                                       3, 2, 6,
                                       3, 6, 7,
                                       1, 0, 4,
                                       1, 4, 5};
std::vector<float> cubeColors {.8f, .4f, .4f, 1.f,
                               .7f, .7f, .4f, 1.f,
                               .4f, .7f, .7f, 1.f,
                               .7f, .4f, .7f, 1.f,
                               .8f, .4f, .4f, 1.f,
                               .7f, .7f, .4f, 1.f,
                               .4f, .7f, .7f, 1.f,
                               .7f, .4f, .7f, 1.f};

std::vector<float> floorVertices {-20.0f, 0.0f, 20.0f,
                                   20.0f, 0.0f, 20.0f,
                                   20.0f, 0.0f, -20.0f,
                                  -20.0f, 0.0f, -20.0f};
std::vector<unsigned int> floorIndices {0, 1, 2,
                                        0, 2, 3};
std::vector<float> floorColors {.2f, .1f, .0f, 1.f,
                                .2f, .1f, .0f, 1.f,
                                .2f, .1f, .0f, 1.f,
                                .2f, .1f, .0f, 1.f};