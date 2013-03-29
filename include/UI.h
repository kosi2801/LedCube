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
        static void addStatusMessage(const char* message);
        
        // activate/deactivate status sections
        static void setStatusWindowActive(bool active);
        static void setCubeWindowActive(bool active);
        static void setMessageWindowActive(bool active);
        
    private:
        static int currentHz;
        static const char* currentAnimationName;
        
        static WINDOW* statusWindow;
        static WINDOW* cubeWindow;
        static WINDOW* messageWindow;
        
        static bool statusWindowActive;
        static bool cubeWindowActive;
        static bool messageWindowActive;
};

#endif // UI_H
