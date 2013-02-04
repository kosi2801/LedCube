#include <ncurses.h>
#include <algorithm>

#include "AnimationSpotControl.h"
#include "CubeConstants.h"

AnimationSpotControl::AnimationSpotControl()
:current_x(0),
 current_y(0),
 current_z(0)
{
    ;
}

long AnimationSpotControl::getFrameMs()
{
    return 100L;
}

const char* AnimationSpotControl::getAnimationName()
{
    return "Spot Control";
}

void AnimationSpotControl::handleKeyPress(int key) {
    switch(key) {
        case KEY_UP:
            current_x++;            
            break;                
        case KEY_DOWN:
            current_x--;
            break;
        case KEY_RIGHT:
            current_y++;
            break;
        case KEY_LEFT:
            current_y--;
            break;
        case KEY_PPAGE:
            current_z--;
            break;
        case KEY_NPAGE:
            current_z++;
            break;
        default:
            break;
    }
    
    current_x = std::max(current_x, 0);
    current_x = std::min(current_x, CUBE_SIZE_X-1);
    
    current_y = std::max(current_y, 0);
    current_y = std::min(current_y, CUBE_SIZE_Y-1);

    current_z = std::max(current_z, 0);
    current_z = std::min(current_z, CUBE_SIZE_LAYERS-1);
}

void AnimationSpotControl::setNextFrame(Cube& cube)
{
    cube.clear();
        
    cube.setVoxel(current_x, current_y, current_z, 255);
}
