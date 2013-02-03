#ifndef ANIMATION_CUBE_PULSE_H
#define ANIMATION_CUBE_PULSE_H

#include "Animation.h"

class AnimationCubePulse : public Animation
{
    public:
        AnimationCubePulse();
        virtual long getFrameMs();
        virtual const char* getAnimationName();
        virtual void setNextFrame(Cube& cube);
        
    private:
        int current_brightness;
        int current_step;
};


#endif // ANIMATION_CUBE_PULSE_H
