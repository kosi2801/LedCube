#include <cstdlib>
#include <ncurses.h>
#include <math.h>
#include <time.h>

#include "AnimationMovingWall.h"
#include "CubeConstants.h"
#include "UI.h"

#define STAGE_1_COUNT 30
#define STAGE_2_COUNT 100
#define STAGE_3_COUNT 20


AnimationMovingWall::AnimationMovingWall()
:currentAnimationCount(0),
 currentStage(0),
 frameMs(100)
{    
    srand(time(NULL));
    
    initNextMove();
}

long AnimationMovingWall::getFrameMs()
{
    return frameMs;
}

const char* AnimationMovingWall::getAnimationName()
{
    return "Moving Wall";
}

void AnimationMovingWall::handleKeyPress(int key) {
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

void AnimationMovingWall::initNextMove() {
    switch(rand()%6+1) {
        case 1: // top to bottom
            currentDirection = Z_NEG;
            break;
        case 2: // bottom to top
            currentDirection = Z_POS;
            break;
        case 3: // left to right
            currentDirection = X_POS;
            break;
        case 4: // right to left
            currentDirection = X_NEG;
            break;
        case 5: // back to front
            currentDirection = Y_NEG;
            break;
        case 6: // front to back
            currentDirection = Y_POS;
            break;
    }
    
    currentStage = 0;
    currentAnimationCount = 0;
}

void AnimationMovingWall::setNextFrame(Cube& cube)
{
    cube.clear();
    ++currentAnimationCount;
    int currentPosition = 0;
    
    // determine overall animation stage&position
    switch (currentStage) {
        case 0: // starting stage, waiting for move. 30 ticks
            switch (currentDirection) {
                case X_POS:
                case Y_POS:
                case Z_POS:
                    currentPosition = 0;
                    break;
                default:
                    currentPosition = CUBE_SIZE_LAYERS -1;
                    break;
            };
            
            // check for next stage
            if(currentAnimationCount >= STAGE_1_COUNT) {
                currentAnimationCount = 0;
                currentStage = 1;
            }
            break;
        case 1: // moving stage. 100 ticks
            currentPosition = trunc(((float)CUBE_SIZE_LAYERS/(float)STAGE_2_COUNT)*(float)currentAnimationCount);
            
            //check for next stage
            if(currentAnimationCount >= STAGE_2_COUNT) {
                currentAnimationCount = 0;
                currentStage = 2;
            }
            break;
        case 2: // cooloff stage, waiting for reset. 20 ticks
            switch (currentDirection) {
                case X_POS:
                case Y_POS:
                case Z_POS:
                    currentPosition = CUBE_SIZE_LAYERS -1;
                    break;
                default:
                    currentPosition = 0;
                    break;
            };
            
            //check for next stage
            if(currentAnimationCount >= STAGE_3_COUNT) {
                initNextMove();
            }
            break;
    };
        
    // light up layer
    switch(currentDirection) {
        case X_POS:
            for(int i = 0; i<CUBE_SIZE_LAYERS; ++i)
                for(int j=0; j<CUBE_SIZE_LAYERS; ++j)
                    cube.setVoxel(currentPosition-1, i, j, 255);
            break;
        case Y_POS:
            for(int i = 0; i<CUBE_SIZE_LAYERS; ++i)
                for(int j=0; j<CUBE_SIZE_LAYERS; ++j)
                    cube.setVoxel(i, currentPosition-1, j, 255);
            break;
        case Z_POS:
            for(int i = 0; i<CUBE_SIZE_LAYERS; ++i)
                for(int j=0; j<CUBE_SIZE_LAYERS; ++j)
                    cube.setVoxel(i, j, currentPosition-1, 255);
            break;
        case X_NEG:
            for(int i = 0; i<CUBE_SIZE_LAYERS; ++i)
                for(int j=0; j<CUBE_SIZE_LAYERS; ++j)
                    cube.setVoxel(CUBE_SIZE_LAYERS-1-currentPosition, i, j, 255);
            break;
        case Y_NEG:
            for(int i = 0; i<CUBE_SIZE_LAYERS; ++i)
                for(int j=0; j<CUBE_SIZE_LAYERS; ++j)
                    cube.setVoxel(i, CUBE_SIZE_LAYERS-1-currentPosition, j, 255);
            break;
        case Z_NEG:
            for(int i = 0; i<CUBE_SIZE_LAYERS; ++i)
                for(int j=0; j<CUBE_SIZE_LAYERS; ++j)
                    cube.setVoxel(i, j, CUBE_SIZE_LAYERS-1-currentPosition, 255);
            break;
    };
    
    char message[200];
    sprintf(message, "Dir %d; Stage %d; AnimationCount %3d; Position %d", currentDirection, currentStage, currentAnimationCount, currentPosition);
    UI::addStatusMessage(message);
}
