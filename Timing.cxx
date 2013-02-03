#include <stdio.h>
#include "Timing.h"
#include "Tools.h"
#include "Cube.h"

Timing::Timing(long target_hz)
:target_hz(target_hz)
{
    setTargetHz(target_hz);
}

Timing::~Timing()
{
    ;
}

void Timing::setTargetHz(long new_target_hz)
{
    // variables required for attaining a stable refresh rate
    const long one_second_usecs = 1000000L;
    target_hz = new_target_hz;
    time_slice = one_second_usecs / (target_hz * C_LAYERS);
    time_offset = 0L;
    
    printf("Target Hz: %d \n\rSlice per Layer: %ld \n\r", target_hz, time_slice);
}

void Timing::startCycles()
{
    gettimeofday(&time_base, 0);
}

void Timing::waitForNextCycle()
{
    // calculate time of next frame, re-sync regularly
//            if(time_offset > 10000000L) {
//                gettimeofday(&time_base, 0);
//                time_offset -= 10000000L;
//            }
    time_offset += time_slice;
    // and wait until time's for next layer
    Tools::wait_until(time_base, time_offset);
}
