#ifndef ANIMATION_RUNNING_LIGHT_H
#define ANIMATION_RUNNING_LIGHT_H

#include "Animation.h"

class AnimationRunningLight : public Animation
{
    public:
        AnimationRunningLight();
        virtual long getFrameMs();
        virtual const char* getAnimationName();
        virtual void setNextFrame(Cube& cube);
        
    private:
        int current_x;
        int current_y;
        int current_z;        
};


#endif // ANIMATION_RUNNING_LIGHT_H
