#ifndef TOOLS_H
#define TOOLS_H

#include "GPIO_Access.h"

class Tools {
    public:
        static void clock_it(GPIO_Access& gpio);
        static void clear_cube(GPIO_Access& gpio);
};

#endif // TOOLS_H
