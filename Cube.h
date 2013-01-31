#define C_LAYERS 3
#define C_X 3
#define C_Y 3

typedef unsigned char uchar;
typedef unsigned int uint;

class Cube {
    public:
     Cube();
     ~Cube();
     void setVoxel(int x, int y, int layer, uchar value);
     uchar getVoxel(int x, int y, int layer);
     
     uint getLayerBitfield(int layer);
     
    
    private:
     uchar cube_status[C_LAYERS][C_Y][C_X];
};
