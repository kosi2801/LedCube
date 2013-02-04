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

