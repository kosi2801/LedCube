#ifndef UI_H
#define UI_H

#include <ncurses.h>

#include "Cube.h"

class UI {
    public:
        // startup and shutdown of UI
        static void init();
        static void shutdown();
    
        // drawing routines
        static void refreshStatus();
        static void refreshCube(const Cube& cube);
        
        // I/O
        static int getKey();
            
        // different status sections
        static void setStatusHz(int hz);
        static void setStatusAnimationName(const char* animationName);
        
    private:
        static int currentHz;
        static const char* currentAnimationName;
        
        static WINDOW* statusWindow;
        static WINDOW* cubeWindow;
        static WINDOW* messageWindow;
};

#endif // UI_H
