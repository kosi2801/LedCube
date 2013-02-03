#include <string>
#include "AnimationPause.h"

AnimationPause::AnimationPause(Animation* animationToPause)
:pausedAnimation(animationToPause),
 pausedAnimationName("<paused>")
{
    
}

AnimationPause::~AnimationPause()
{
    if(pausedAnimation != NULL)
        delete pausedAnimation;
}

Animation* AnimationPause::getPausedAnimation()
{
    return pausedAnimation;
}

void AnimationPause::setPausedAnimation(Animation* animation)
{
    pausedAnimation = animation;
}

long AnimationPause::getFrameMs()
{
    return 50L;
}

const char* AnimationPause::getAnimationName()
{    
    if(pausedAnimationName.length() <= 8) // length of "<paused>"
        pausedAnimationName.append(pausedAnimation->getAnimationName());
    return pausedAnimationName.c_str();
}

void AnimationPause::setNextFrame(Cube& cube)
{
    ; // pause does nothing
}
