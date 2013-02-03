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
    time_slice = one_second_usecs / (target_hz * CUBE_SIZE_LAYERS);
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

timeval Timing::getFutureTime(const long ms_from_now)
{
    struct timeval target;
    
    gettimeofday(&target, 0);

    // calculate future time
    target.tv_sec += (ms_from_now/1000L);
    target.tv_usec += ((ms_from_now%1000L)*1000L);
    if(target.tv_usec > 1000000L) 
    {
        ++target.tv_sec;
        target.tv_usec -= 1000000L;
    }
    
    return target;
}

timeval Timing::getNow()
{
    struct timeval now;
    
    gettimeofday(&now, 0);
    
    return now;
}

bool Timing::isTimeReached(timeval& target_time)
{
    struct timeval now;    
    gettimeofday(&now, 0);
    
    if(now.tv_sec > target_time.tv_sec)
        return true;
    if(now.tv_sec < target_time.tv_sec)
        return false;        
    if(now.tv_usec < target_time.tv_usec)
        return false;
    return true;
        
}

