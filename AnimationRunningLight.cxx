#include "AnimationRunningLight.h"

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
    
    if(current_x > 2) {
        ++current_y;
        current_x = 0;
    }
    
    if(current_y > 2) {
        ++current_z;
        current_y = 0;
    }
    
    if(current_z > 2) {
        current_z = 0;
    }
    
    cube.setVoxel(current_x, current_y, current_z, 255);
}
