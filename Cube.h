#ifndef CUBE_H
#define CUBE_H

#define C_LAYERS 3
#define C_X      3
#define C_Y      3

#define CLK   GPIO_0
#define LATCH GPIO_1
#define SD1   GPIO_7
#define SD2   GPIO_8

typedef unsigned char uchar;
typedef unsigned int uint;

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
     
     uchar cube_status[C_LAYERS][C_Y][C_X];
};

#endif // CUBE_H
