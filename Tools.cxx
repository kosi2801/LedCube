#include <sys/time.h>
#include "Tools.h"
#include "Cube.h"

void Tools::clock_it(GPIO_Access& gpio) 
{
    gpio.clearPort(CLK);
    gpio.setPort(CLK);
}

void Tools::clear_cube(GPIO_Access& gpio)
{
    // LE off
    gpio.clearPort(LATCH);
        
    gpio.clearPort(SD1);
    for(int i=0; i<16; ++i) {
        clock_it(gpio);
    }
    
    // LE on
    gpio.setPort(LATCH);
}

void Tools::wait_usecs(const int delay_micros)
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

void Tools::wait_until(const timeval& time_base, long offset_micros)
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
