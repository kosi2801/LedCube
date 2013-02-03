#ifndef TIMING_H
#define TIMING_H

#include <sys/time.h>

class Timing
{
    public:
        Timing(long target_hz = 5000L);
        ~Timing();
        
        void setTargetHz(long new_target_hz);
        void startCycles();
        void waitForNextCycle();
        
    private:        
        long target_hz;
        long time_slice;
        timeval time_base;
        long time_offset;

};

#endif // TIMING_H