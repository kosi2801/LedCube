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
     
     void refreshCubeForBamPosition(uchar bamPosition);
     const bool getVoxelStatus(int x, int y, int layer) const;
     
    
    private:
     const uchar getBamBitForPosition(const uchar x) const;
     
     uchar cube_status[CUBE_SIZE_LAYERS][CUBE_SIZE_Y][CUBE_SIZE_X];
     bool cube_cache[CUBE_SIZE_LAYERS][CUBE_SIZE_Y][CUBE_SIZE_X];
};

#endif // CUBE_H
