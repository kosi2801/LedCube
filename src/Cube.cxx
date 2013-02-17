#include <stdio.h>

#include "Cube.h"
#include "CubeConstants.h"
#include "UI.h"

Cube::Cube() {
    for(int layer = 0; layer < CUBE_SIZE_LAYERS; ++layer)
        for(int y = 0; y < CUBE_SIZE_Y; ++y)
            for(int x = 0; x < CUBE_SIZE_X; ++x)
                cube_status[layer][y][x] = 0;
}

Cube::~Cube() {
    ;
}

void Cube::setVoxel(int x, int y, int layer, uchar value) {
    cube_status[layer][y][x] = value;
}

const uchar Cube::getVoxel(int x, int y, int layer) const {
    return cube_status[layer][y][x];
}

void Cube::clear() {
    for(int layer = 0; layer < CUBE_SIZE_LAYERS; ++layer)
        for(int y = 0; y < CUBE_SIZE_Y; ++y)
            for(int x = 0; x < CUBE_SIZE_X; ++x)
                cube_status[layer][y][x] = 0;
}
    
void Cube::printStatus() {
    UI::refreshCube(*this);        
}

const uint Cube::getLayerBitfield(int layer) const {
    uint result = 0;
    for(int y = 0; y < CUBE_SIZE_Y; ++y)
        for(int x = 0; x < CUBE_SIZE_X; ++x)
            result |= (cube_status[layer][y][x]?1:0) << ((y*CUBE_SIZE_X) + x);

    return result;
}

const uint Cube::getLayerBamBitfield(int layer, uchar bamPosition) const {
    uint result = 0;
    const uchar currentBamBit = getBamBitForPosition(bamPosition);
    for(int y = 0; y < CUBE_SIZE_Y; ++y)
        for(int x = 0; x < CUBE_SIZE_X; ++x)
            result |= ((cube_status[layer][y][x] & currentBamBit)?1:0) << ((y*CUBE_SIZE_X) + x);

    return result;
}

const uchar Cube::getBamBitForPosition(const uchar x) const {
    static const uchar msb_lut[256] =
        {
             0, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             8, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             4, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             8, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             2, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             8, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             4, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             8, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             1, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             8, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             4, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             8, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             2, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             8, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             4, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128,
             8, 128,  64, 128,  32, 128,  64, 128,
            16, 128,  64, 128,  32, 128,  64, 128
        };

	return msb_lut[x];
}
