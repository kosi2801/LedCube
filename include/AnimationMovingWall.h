#ifndef ANIMATION_MOVING_WALL_H
#define ANIMATION_MOVING_WALL_H

#include "Animation.h"

class AnimationMovingWall : public Animation
{
    public:
        AnimationMovingWall();
        virtual long getFrameMs();
        virtual const char* getAnimationName();
        virtual void setNextFrame(Cube& cube);
        virtual void handleKeyPress(int key);
        
    private:
        void initNextMove();
        
        enum WallDirection {
            X_POS,
            Y_POS,
            Z_POS,
            X_NEG,
            Y_NEG,
            Z_NEG
        };
        
        WallDirection currentDirection;
        int currentAnimationCount;
        int currentStage;
        int frameMs;
};


#endif // ANIMATION_MOVING_WALL_H
