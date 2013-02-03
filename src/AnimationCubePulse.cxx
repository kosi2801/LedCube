#include "AnimationCubePulse.h"
#include "CubeConstants.h"

AnimationCubePulse::AnimationCubePulse()
:current_brightness(0),
 current_step(10)
{
    ;
}

long AnimationCubePulse::getFrameMs()
{
    return 50L;
}

const char* AnimationCubePulse::getAnimationName()
{
    return "Cube Pulse";
}

void AnimationCubePulse::setNextFrame(Cube& cube)
{
    if(current_brightness < 10)
        current_step = 10;
    else if (current_brightness > 240)
        current_step = -10;

    current_brightness += current_step;
    
    for(int x=0; x<CUBE_SIZE_X; ++x)
        for(int y=0; y<CUBE_SIZE_Y; ++y)
            for(int z=0; z<CUBE_SIZE_LAYERS; ++z)
                cube.setVoxel(x, y, z, current_brightness);
}
