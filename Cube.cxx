#include "Cube.h"

Cube::Cube() {
    for(int layer = 0; layer < C_LAYERS; ++layer)
        for(int y = 0; y < C_Y; ++y)
            for(int x = 0; x < C_X; ++x)
                cube_status[layer][y][x] = 0;
}

Cube::~Cube() {
    ;
}

void Cube::setVoxel(int x, int y, int layer, uchar value) {
    cube_status[layer][y][x] = value;
}

uchar Cube::getVoxel(int x, int y, int layer) {
    return cube_status[layer][y][x];
}

uint Cube::getLayerBitfield(int layer) {
    uint result = 0;
    for(int y = 0; y < C_Y; ++y)
        for(int x = 0; x < C_X; ++x)
            result |= cube_status[layer][y][x] << ((y*C_X) + x);
            
    return result;
}

