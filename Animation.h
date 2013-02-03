#ifndef ANIMATION_H
#define ANIMATION_H

#include "Cube.h"

class Animation
{
    public:
        virtual long getFrameMs() = 0;
        virtual void setNextFrame(Cube& cube) = 0;
};

#endif // ANIMATION_H
