#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>

#include "GPIO_Access.h"
#include "Cube.h"
#include "Tools.h"
#include "Timing.h"

using namespace std;

int main(int argc, char **argv)
{
    long target_hz = 5000L;
    
    // first argument is target refresh rate
    if(argc > 1) {
        target_hz = atol(argv[1]);
    }
    
    Timing timing(target_hz);
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
    cube.setVoxel(0,0,0,255);
    cube.setVoxel(1,1,1,255);
    cube.setVoxel(2,2,2,255);

/*    for(int x=0; x<3; ++x)
        for(int y=0; y<3; ++y)
            for(int z=0; z<3; ++z)
                cube.setVoxel(x, y, z, (9*x)+(9*3*y)+(9*3*3*z)+1);
    */
    
    // required for performance measurement
    timeval current_time, last_second;    
    int last_count = 0;
    
    // initialize timing status
    timing.startCycles();
    
    for(int count=0;;++count) {        
        // clock out layer
        for(int layer = 0; layer < C_LAYERS; ++layer) {            
//            Tools::clear_cube(gpio);
            
            // LE off
            gpio.clearPort(LATCH);
        
            // 4 low pins padding
            gpio.clearPort(SD1);
            for(int i=0; i<4; ++i) {
                Tools::clock_it(gpio);
            }
            
            // 3 layer pins
            uint layer_bit = 1 << layer;
            layer_bit&(1<<0) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
            Tools::clock_it(gpio);
            layer_bit&(1<<1) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
            Tools::clock_it(gpio);
            layer_bit&(1<<2) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
            Tools::clock_it(gpio);

            // 9 cube pins
            // if target Hz>255 enable BAM
            uint cube_bits;
            if(target_hz > 255)
            {
                cube_bits = cube.getLayerBamBitfield(layer, count%256);
            } else {
                cube_bits = cube.getLayerBitfield(layer);
            }
            
            for(int i=0; i<9; ++i) {
                (cube_bits & (1<<i)) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
                Tools::clock_it(gpio);
            }
            
            // LE on
            gpio.setPort(LATCH);
            
            // delay until next period
            timing.waitForNextCycle();
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
