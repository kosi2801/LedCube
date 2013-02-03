#include <stdio.h>
#include <ncurses.h>

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

void Cube::clear() {
    for(int layer = 0; layer < C_LAYERS; ++layer)
        for(int y = 0; y < C_Y; ++y)
            for(int x = 0; x < C_X; ++x)
                cube_status[layer][y][x] = 0;
}
    
void Cube::printStatus() {
    mvprintw(2,0,"%3d %3d %3d   %3d %3d %3d   %3d %3d %3d", 
        cube_status[0][0][0], cube_status[0][0][1], cube_status[0][0][2], 
        cube_status[1][0][0], cube_status[1][0][1], cube_status[1][0][2], 
        cube_status[2][0][0], cube_status[2][0][1], cube_status[2][0][2]);
    mvprintw(4,0,"%3d %3d %3d   %3d %3d %3d   %3d %3d %3d", 
        cube_status[0][1][0], cube_status[0][1][1], cube_status[0][1][2], 
        cube_status[1][1][0], cube_status[1][1][1], cube_status[1][1][2], 
        cube_status[2][1][0], cube_status[2][1][1], cube_status[2][1][2]);
    mvprintw(6,0,"%3d %3d %3d   %3d %3d %3d   %3d %3d %3d", 
        cube_status[0][2][0], cube_status[0][2][1], cube_status[0][2][2], 
        cube_status[1][2][0], cube_status[1][2][1], cube_status[1][2][2], 
        cube_status[2][2][0], cube_status[2][2][1], cube_status[2][2][2]);
        
}

uint Cube::getLayerBitfield(int layer) {
    uint result = 0;
    for(int y = 0; y < C_Y; ++y)
        for(int x = 0; x < C_X; ++x)
            result |= (cube_status[layer][y][x]?1:0) << ((y*C_X) + x);

    return result;
}

uint Cube::getLayerBamBitfield(int layer, uchar bamPosition) {
    uint result = 0;
    unsigned char currentBamBit = getBamBitForPosition(bamPosition);
    for(int y = 0; y < C_Y; ++y)
        for(int x = 0; x < C_X; ++x)
            result |= ((cube_status[layer][y][x] & currentBamBit)?1:0) << ((y*C_X) + x);

    return result;
}

const uchar Cube::getBamBitForPosition(const uchar x) {
    static const uchar msb_lut[256] =
        {
              0,   1,   2,   4,   8,  16,  32,  64,
            128, 128,  64, 128, 128,  32,  64, 128,
            128,  64, 128, 128,  16,  32,  64, 128,
            128,  64, 128, 128,  32,  64, 128, 128,
             64, 128, 128,   8,  16,  32,  64, 128,
            128,  64, 128, 128,  32,  64, 128, 128,
             64, 128, 128,  16,  32,  64, 128, 128,
             64, 128, 128,  32,  64, 128, 128,  64,
            128, 128,   4,   8,  16,  32,  64, 128,
            128,  64, 128, 128,  32,  64, 128, 128,
             64, 128, 128,  16,  32,  64, 128, 128,
             64, 128, 128,  32,  64, 128, 128,  64,
            128, 128,   8,  16,  32,  64, 128, 128,
             64, 128, 128,  32,  64, 128, 128,  64,
            128, 128,  16,  32,  64, 128, 128,  64,
            128, 128,  32,  64, 128, 128,  64, 128,
            128,   2,   4,   8,  16,  32,  64, 128,
            128,  64, 128, 128,  32,  64, 128, 128,
             64, 128, 128,  16,  32,  64, 128, 128,
             64, 128, 128,  32,  64, 128, 128,  64,
            128, 128,   8,  16,  32,  64, 128, 128,
             64, 128, 128,  32,  64, 128, 128,  64,
            128, 128,  16,  32,  64, 128, 128,  64,
            128, 128,  32,  64, 128, 128,  64, 128,
            128,   4,   8,  16,  32,  64, 128, 128,
             64, 128, 128,  32,  64, 128, 128,  64,
            128, 128,  16,  32,  64, 128, 128,  64,
            128, 128,  32,  64, 128, 128,  64, 128,
            128,   8,  16,  32,  64, 128, 128,  64,
            128, 128,  32,  64, 128, 128,  64, 128,
            128,  16,  32,  64, 128, 128,  64, 128,
            128,  32,  64, 128, 128,  64, 128, 128,
        };

	return msb_lut[x];
}
