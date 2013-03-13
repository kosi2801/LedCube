#ifndef TIMING_H
#define TIMING_H

#include "GPIO_Access.h"

class Timing
{
    public:
        Timing(long target_hz = 5000L);
        ~Timing();
        
        static void wait_usecs(const uint64_t delay_micros);
        static long wait_until(const uint64_t time_base, uint64_t offset_micros);

        void setTargetHz(long new_target_hz);
        void startCycles();
        long waitForNextCycle();
        uint64_t getNow();
        uint64_t getFutureTime(long ms_from_now);
        bool isTimeReached(const uint64_t target_time);
        
    private:        
        long target_hz;
        long time_slice;
        uint64_t time_base;
        long time_offset;
        
        static GPIO_Access gpio;
};

#endif // TIMING_H
