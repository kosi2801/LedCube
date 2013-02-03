#ifndef CUBE_H
#define CUBE_H

#include "CubeConstants.h"

class Cube {
    public:
     Cube();
     ~Cube();
     void setVoxel(int x, int y, int layer, uchar value);
     uchar getVoxel(int x, int y, int layer);
     void clear();
     void printStatus();
     
     uint getLayerBitfield(int layer);
     uint getLayerBamBitfield(int layer, unsigned char bamPosition);
     
    
    private:
     const uchar getBamBitForPosition(const uchar x);
     
     uchar cube_status[CUBE_SIZE_LAYERS][CUBE_SIZE_Y][CUBE_SIZE_X];
};

#endif // CUBE_H
