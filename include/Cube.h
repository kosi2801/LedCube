#ifndef CUBE_H
#define CUBE_H

#include "CubeConstants.h"

class Cube {
    public:
     Cube();
     ~Cube();
     void setVoxel(int x, int y, int layer, uchar value);
     const uchar getVoxel(int x, int y, int layer) const;
     void clear();
     void printStatus();
     
     const uint getLayerBitfield(int layer) const;
     const uint getLayerBamBitfield(int layer, unsigned char bamPosition) const;
     
    
    private:
     const uchar getBamBitForPosition(const uchar x) const;
     
     uchar cube_status[CUBE_SIZE_LAYERS][CUBE_SIZE_Y][CUBE_SIZE_X];
};

#endif // CUBE_H
