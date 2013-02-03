#include "AnimationRunningLight.h"
#include "CubeConstants.h"

AnimationRunningLight::AnimationRunningLight()
:current_x(0),
 current_y(0),
 current_z(0)
{
    ;
}

long AnimationRunningLight::getFrameMs()
{
    return 250L;
}

const char* AnimationRunningLight::getAnimationName()
{
    return "Running Light";
}

void AnimationRunningLight::setNextFrame(Cube& cube)
{
    cube.clear();
    
    ++current_x;
    
    if(current_x >= CUBE_SIZE_X) {
        ++current_y;
        current_x = 0;
    }
    
    if(current_y >= CUBE_SIZE_Y) {
        ++current_z;
        current_y = 0;
    }
    
    if(current_z >= CUBE_SIZE_LAYERS) {
        current_z = 0;
    }
    
    cube.setVoxel(current_x, current_y, current_z, 255);
}
