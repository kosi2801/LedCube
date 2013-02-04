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

void Timing::wait_usecs(const int delay_micros)
{
    int micros = 0;
    struct timeval pulse, now;

    // wait until number of usecs to wait for have passed
    gettimeofday(&pulse, 0);    
    while(micros < delay_micros)
    {
        gettimeofday(&now, 0);
        micros = (now.tv_sec - pulse.tv_sec) * 1000000L;
        micros = micros + (now.tv_usec - pulse.tv_usec);        
    }
    
    return;
}

void Timing::wait_until(const timeval& time_base, long offset_micros)
{
    struct timeval now, target;
    
    // calculate time to wait for
    target.tv_sec = time_base.tv_sec + (offset_micros/1000000L);
    target.tv_usec = time_base.tv_usec + (offset_micros%1000000L);
    if(target.tv_usec > 1000000L) 
    {
        ++target.tv_sec;
        target.tv_usec -= 1000000L;
    }
    
    // wait for correct second
    gettimeofday(&now, 0);
    while(now.tv_sec < target.tv_sec)
        gettimeofday(&now, 0);
    
    // wait for corrent microsecond
    while((now.tv_usec < target.tv_usec) && (now.tv_sec == target.tv_sec))
        gettimeofday(&now, 0);
        
    return;
}

void Timing::setTargetHz(long new_target_hz)
{
    // variables required for attaining a stable refresh rate
    const long one_second_usecs = 1000000L;
    target_hz = new_target_hz;
    time_slice = one_second_usecs / (target_hz * CUBE_SIZE_LAYERS);
    time_offset = 0L;
    
    printf("Target Hz: %ld \n\rSlice per Layer: %ld \n\r", target_hz, time_slice);
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
    Timing::wait_until(time_base, time_offset);
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

