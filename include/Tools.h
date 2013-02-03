#ifndef TOOLS_H
#define TOOLS_H

#include "GPIO_Access.h"

class Tools {
    public:
        static void clock_it(GPIO_Access& gpio);
        static void clear_cube(GPIO_Access& gpio);
        static void wait_usecs(const int delay_micros);
        static void wait_until(const timeval& time_base, long offset_micros);
};

#endif // TOOLS_H
