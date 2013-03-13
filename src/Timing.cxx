#include <stdio.h>
#include <sched.h>
#include <time.h>
#include "Timing.h"
#include "Tools.h"
#include "Cube.h"

GPIO_Access Timing::gpio;

Timing::Timing(long target_hz)
:target_hz(target_hz)
{
    setTargetHz(target_hz);
}

Timing::~Timing()
{
    ;
}

void Timing::wait_usecs(const uint64_t delay_micros)
{
    uint64_t start, target;
    
    start = gpio.getSystemTimer();
    
    // Calling nanosleep() takes at least 100-200us, so use it for
    // long waits and use a busy wait on the System Timer for the rest.
    if (delay_micros > 450)
    {
        struct timespec t1;
        t1.tv_sec = 0;
        t1.tv_nsec = 1000 * (long)(delay_micros - 200);
        nanosleep(&t1, NULL);
    }
    
    target = start + delay_micros;

    
    while(gpio.getSystemTimer() < target) 
    ;
}

long Timing::wait_until(const uint64_t time_base, uint64_t offset_micros)
{
    uint64_t now = gpio.getSystemTimer();
    uint64_t target = time_base + offset_micros;
    
    if(now<target)
        wait_usecs(target - now);
        
    return target-now;
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
    time_base = gpio.getSystemTimer();
}

long Timing::waitForNextCycle()
{
    // calculate time of next frame
    time_offset += time_slice;
    
    // and wait until time's for next layer
    return Timing::wait_until(time_base, time_offset);
}

uint64_t Timing::getFutureTime(const long ms_from_now)
{
    return gpio.getSystemTimer() + (ms_from_now * 1000L);
}

uint64_t Timing::getNow()
{
    return gpio.getSystemTimer();
}

bool Timing::isTimeReached(uint64_t target_time)
{
    return (target_time <= gpio.getSystemTimer());
}

