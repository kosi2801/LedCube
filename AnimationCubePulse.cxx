#include "AnimationCubePulse.h"

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
    
    for(int x=0; x<3; ++x)
        for(int y=0; y<3; ++y)
            for(int z=0; z<3; ++z)
                cube.setVoxel(x, y, z, current_brightness);
}
