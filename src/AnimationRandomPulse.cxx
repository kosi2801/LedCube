#include <cstdlib>
#include <algorithm>
#include <ncurses.h>
#include <time.h>

#include "AnimationRandomPulse.h"
#include "CubeConstants.h"

AnimationRandomPulse::AnimationRandomPulse()
:current_x(0),
 current_y(0),
 current_z(0),
 frameMs(20L),
 brightness(0),
 periodPosition(255)
{    
    srand(time(NULL));
}

long AnimationRandomPulse::getFrameMs()
{
    return frameMs;
}

const char* AnimationRandomPulse::getAnimationName()
{
    return "Random Pulse";
}

void AnimationRandomPulse::handleKeyPress(int key) {
    switch(key) {
        case KEY_RIGHT:
            frameMs -= 10;
            if(frameMs <= 0)
                frameMs = 0;
            break;
        case KEY_LEFT:
            frameMs += 10;
            break;
        default:
            break;
    }
}

void AnimationRandomPulse::setNextFrame(Cube& cube)
{
    ++periodPosition;
    
    if(periodPosition >= 255) {
        cube.clear();
        current_x = rand() % CUBE_SIZE_X;
        current_y = rand() % CUBE_SIZE_Y;
        current_z = rand() % CUBE_SIZE_LAYERS;
        brightness = 0;
        periodPosition = 0;        
    }
    
    if(periodPosition <= 128) 
        brightness = std::min(brightness+2, 255);
    else
        brightness = std::max(brightness-2, 0);
    
    cube.setVoxel(current_x, current_y, current_z, brightness);
}
