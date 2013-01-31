#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>

#include "GPIO_Access.h"
#include "Cube.h"

using namespace std;

#define CLK GPIO_0
#define LATCH GPIO_1
#define SD1 GPIO_7
#define SD2 GPIO_8

void clock_it(GPIO_Access& gpio) 
{
    gpio.clearPort(CLK);
    gpio.setPort(CLK);
}

void clear_cube(GPIO_Access& gpio)
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

void wait_usecs(int delay_micros)
{
    int micros = 0;
    struct timeval pulse, now;
    gettimeofday(&pulse, 0);
    
    while(micros < delay_micros)
    {
        gettimeofday(&now, 0);
        if(now.tv_sec > pulse.tv_sec) micros=1000000L; else micros=0;
        micros = micros + (now.tv_usec - pulse.tv_usec);        
    }
    
    return;
}

void wait_until(const timeval& time_base, long offset_micros)
{
    struct timeval now, target;
    gettimeofday(&now, 0);
    
    target.tv_sec = time_base.tv_sec + (offset_micros/1000000L);
    target.tv_usec = time_base.tv_usec + (offset_micros%1000000L);
    if(target.tv_usec > 1000000L) 
    {
        ++target.tv_sec;
        target.tv_usec -= 1000000L;
    }
        
//    printf("now   : %ld secs, %ld usecs \r\n", now.tv_sec, now.tv_usec);
//    printf("target: %ld secs, %ld usecs \r\n", target.tv_sec, target.tv_usec);
    
    while(now.tv_sec < target.tv_sec)
        gettimeofday(&now, 0);
    
    while((now.tv_usec < target.tv_usec) && (now.tv_sec == target.tv_sec))
        gettimeofday(&now, 0);
        
    return;
}

int main(int argc, char **argv)
{
    
    // variables required for attaining a stable refresh rate
    const long one_second_usecs = 1000000L;
    long target_hz = 5000L;
    long time_slice = one_second_usecs / (target_hz * C_LAYERS);
    timeval time_base;
    long time_offset = 0L;
    
    // first argument is target refresh rate
    if(argc > 1) {
        target_hz = atol(argv[1]);
        time_slice = one_second_usecs / (target_hz * C_LAYERS);
    }
    
    GPIO_Access gpio;
    
    gpio.setMode(CLK, GPIO_OUT);
    gpio.setMode(LATCH, GPIO_OUT);
    gpio.setMode(SD1, GPIO_OUT);
    gpio.setMode(SD2, GPIO_OUT);
    
    // LE on, CLK off, SD1 off
    gpio.setPort(LATCH);
    gpio.clearPort(CLK);
    gpio.clearPort(SD1);    
    gpio.clearPort(SD2);    
    
    // init cube and set some voxels
    Cube cube;
    cube.setVoxel(0,0,0,1);
    cube.setVoxel(1,1,1,1);
    cube.setVoxel(2,2,2,1);
    
    // required for performance measurement
    timeval current_time, last_second;
    
    int last_count = 0;
    
    gettimeofday(&time_base, 0);
    
    printf("Target Hz: %d \n\rSlice per Layer: %ld \n\r", target_hz, time_slice);
    
    for(int count=0;;++count) {        
        // clock out layer
        for(int layer = 0; layer < C_LAYERS; ++layer) {
            // calculate time of next frame, re-sync regularly
//            if(time_offset > 10000000L) {
//                gettimeofday(&time_base, 0);
//                time_offset -= 10000000L;
//            }
            time_offset += time_slice;
            // and wait until time's for next layer
            wait_until(time_base, time_offset);
            
            clear_cube(gpio);
            
            // LE off
            gpio.clearPort(LATCH);
        
            // 4 low pins padding
            gpio.clearPort(SD1);
            for(int i=0; i<4; ++i) {
                clock_it(gpio);
            }
            
            // 3 layer pins
            uint layer_bit = 1 << layer;
            layer_bit&(1<<0) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
            clock_it(gpio);
            layer_bit&(1<<1) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
            clock_it(gpio);
            layer_bit&(1<<2) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
            clock_it(gpio);

            // 9 cube pins
            uint cube_bits = cube.getLayerBitfield(layer);            
            for(int i=0; i<9; ++i) {
                (cube_bits & (1<<i)) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
                clock_it(gpio);
            }
            
            // LE on
            gpio.setPort(LATCH);
        }
        
        // performance measurement
        gettimeofday(&current_time, 0);
        if(current_time.tv_sec > last_second.tv_sec) {
            printf("%d Hz in the last second \r\n", count-last_count);
             
            last_second = current_time;
            last_count = count;
        }
        
    }
} 
