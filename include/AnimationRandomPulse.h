#ifndef ANIMATION_RANDOM_PULSE_H
#define ANIMATION_RUNNING_LIGHT_H

#include "Animation.h"

class AnimationRandomPulse : public Animation
{
    public:
        AnimationRandomPulse();
        virtual long getFrameMs();
        virtual const char* getAnimationName();
        virtual void setNextFrame(Cube& cube);
        virtual void handleKeyPress(int key);
        
    private:
        int current_x;
        int current_y;
        int current_z;        
        
        long frameMs;
        int brightness;
        int periodPosition;
};


#endif // ANIMATION_RANDOM_PULSE_H
