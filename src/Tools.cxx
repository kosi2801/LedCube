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

void Tools::drain_cube(GPIO_Access& gpio)
{
    // at first, turn off everything
    Tools::clear_cube(gpio);
    
    // LE off
    gpio.clearPort(LATCH);
        
    // keep off padding pins and layer transistors
    gpio.clearPort(SD1);
    for(int i=0; i<7; ++i) {
        clock_it(gpio);
    }

    // set sink ports for LEDs to enabled, let remaining power flow out
    gpio.setPort(SD1);
    for(int i=0; i<9; ++i) {
        clock_it(gpio);
    }    
    
    // LE on
    gpio.setPort(LATCH);
}
