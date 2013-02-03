#ifndef ANIMATION_PAUSE_H
#define ANIMATION_PAUSE_H

#include <string>
#include "Animation.h"

class AnimationPause : public Animation
{
    public:
        AnimationPause(Animation* animationToPause);
        ~AnimationPause();
        virtual long getFrameMs();
        virtual const char* getAnimationName();
        virtual void setNextFrame(Cube& cube);
        Animation* getPausedAnimation();
        void setPausedAnimation();
        
    private:
        Animation* pausedAnimation;
        std::string pausedAnimationName;
};


#endif // ANIMATION_PAUSE_H
