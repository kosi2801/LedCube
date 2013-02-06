#ifndef MAIN_H
#define MAIN_H

#include "Animation.h"

class Main {
    public:
        Main();
        ~Main();
        int run(int argc, char **argv);
        void doInputCheck();
        static void finish(int sig);
        
    private:
        static bool isFinished();
        
        Animation* animation;
        static bool finished;
        static int exitCode;
};

#endif // MAIN_H
