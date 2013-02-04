#include <ncurses.h>

#include "AnimationRunningLight.h"
#include "CubeConstants.h"

AnimationRunningLight::AnimationRunningLight()
:current_x(0),
 current_y(0),
 current_z(0),
 frameMs(200L),
 brightness(255)
{
    ;
}

long AnimationRunningLight::getFrameMs()
{
    return frameMs;
}

const char* AnimationRunningLight::getAnimationName()
{
    return "Running Light";
}

void AnimationRunningLight::handleKeyPress(int key) {
    switch(key) {
        case KEY_UP:
            brightness += 20;
            if(brightness > 255)
                brightness = 255;
            break;                
        case KEY_DOWN:
            brightness -= 20;
            if(brightness < 1)
                brightness = 1;
            break;
        case KEY_RIGHT:
            frameMs -= 10;
            if(frameMs <= 0)
                frameMs = 0;
            break;
        case KEY_LEFT:
            frameMs += 10;
            break;
        case KEY_PPAGE:
            frameMs -= 50;
            if(frameMs <= 0)
                frameMs = 0;
            break;
        case KEY_NPAGE:
            frameMs += 50;
            break;
        default:
            break;
    }
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
    
    cube.setVoxel(current_x, current_y, current_z, brightness);
}
