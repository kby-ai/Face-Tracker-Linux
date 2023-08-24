#ifndef DEFINITION_H
#define DEFINITION_H

#include <string>

struct FaceBox {
    float confidence;
    float x1;
    float y1;
    float x2;
    float y2;
};

struct ModelConfig {
    float scale;
    float shift_x;
    float shift_y;
    int height;
    int width;
    std::string name;
    bool org_resize;
};

struct Track {
    int id;
    float score;
    float x1;
    float y1;
    float x2;
    float y2;
};

#endif //DEFINITION_H
