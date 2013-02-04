#ifndef ANIMATION_SPOT_CONTROL_H
#define ANIMATION_SPOT_CONTROL_H

#include "Animation.h"

class AnimationSpotControl : public Animation
{
    public:
        AnimationSpotControl();
        virtual long getFrameMs();
        virtual const char* getAnimationName();
        virtual void setNextFrame(Cube& cube);
        virtual void handleKeyPress(int key);
        
    private:
        int current_x;
        int current_y;
        int current_z;        
};


#endif // ANIMATION_SPOT_CONTROL_H
