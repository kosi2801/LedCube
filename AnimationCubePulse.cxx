#include <stdio.h>
#include "AnimationCubePulse.h"

AnimationCubePulse::AnimationCubePulse()
:current_brightness(0),
 current_step(20)
{
    ;
}

long AnimationCubePulse::getFrameMs()
{
    return 100L;
}

void AnimationCubePulse::setNextFrame(Cube& cube)
{
    if(current_brightness < 20)
        current_step = 20;
    else if (current_brightness > 230)
        current_step = -20;

    current_brightness += current_step;
    //printf("New brightness: %ld\n\r", current_brightness);

    
    for(int x=0; x<3; ++x)
        for(int y=0; y<3; ++y)
            for(int z=0; z<3; ++z)
                cube.setVoxel(x, y, z, current_brightness);
}
