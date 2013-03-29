#include <signal.h>

#include "UI.h"


int UI::currentHz = 0;
const char* UI::currentAnimationName = NULL;
WINDOW* UI::statusWindow = NULL;
WINDOW* UI::cubeWindow = NULL;
WINDOW* UI::messageWindow = NULL;

bool UI::statusWindowActive = true;
bool UI::cubeWindowActive = true;
bool UI::messageWindowActive = true;

void UI::init() {
    initscr();
    //signal(SIGINT, finish);
    nodelay(stdscr, TRUE);
    cbreak();
    noecho();
    clear();
    keypad(stdscr, TRUE);
    
    UI::currentHz = 0;
    UI::currentAnimationName = "<none>";
    
    UI::statusWindow = NULL;
    UI::cubeWindow = NULL;
    UI::messageWindow = NULL;
    
    UI::statusWindow = newwin(4,60,0,0);
    UI::cubeWindow = newwin(20, 60, 6, 0);
    UI::messageWindow = newwin(20, 60, 30, 0);
    
    scrollok(UI::messageWindow, true);
}

void UI::shutdown() {
    delwin(statusWindow);
    delwin(cubeWindow);
    delwin(messageWindow);
    
    endwin();
}

void UI::setStatusWindowActive(bool active) {
    statusWindowActive = active;
}

void UI::setCubeWindowActive(bool active) {
    cubeWindowActive = active;
}

void UI::setMessageWindowActive(bool active) {
    messageWindowActive = active;
}

void UI::setStatusHz(int hz) {
    currentHz = hz;
}

void UI::setStatusAnimationName(const char* animationName) {
    currentAnimationName = animationName;
}

void UI::addStatusMessage(const char* message) {
    if(!messageWindowActive) return;
    
    wprintw(messageWindow, "%s\n", message);
    wrefresh(messageWindow);
}

void UI::refreshStatus() {
    if(!statusWindowActive) return;
    
    // update Status window
    wclear(statusWindow);
    wmove(statusWindow, 0, 0);
    wprintw(statusWindow, "%d Hz in the last second\n", currentHz);
    wprintw(statusWindow, "Current animation: %s", currentAnimationName);    
    wrefresh(statusWindow);
}

int UI::getKey() {
    int ret = getch();
    flushinp();
    return ret;
}

void UI::refreshCube(const Cube& cube) {
    if(!cubeWindowActive) return;
    
    // update Cube window
    wclear(cubeWindow);
    wmove(cubeWindow, 0, 0);
    
    wprintw(cubeWindow,"%3d %3d %3d   %3d %3d %3d   %3d %3d %3d\n\n", 
        cube.getVoxel(0, 2, 0), cube.getVoxel(1, 2, 0), cube.getVoxel(2, 2, 0), 
        cube.getVoxel(0, 2, 1), cube.getVoxel(1, 2, 1), cube.getVoxel(2, 2, 1), 
        cube.getVoxel(0, 2, 2), cube.getVoxel(1, 2, 2), cube.getVoxel(2, 2, 2));
    wprintw(cubeWindow,"%3d %3d %3d   %3d %3d %3d   %3d %3d %3d\n\n", 
        cube.getVoxel(0, 1, 0), cube.getVoxel(1, 1, 0), cube.getVoxel(2, 1, 0), 
        cube.getVoxel(0, 1, 1), cube.getVoxel(1, 1, 1), cube.getVoxel(2, 1, 1), 
        cube.getVoxel(0, 1, 2), cube.getVoxel(1, 1, 2), cube.getVoxel(2, 1, 2));
    wprintw(cubeWindow,"%3d %3d %3d   %3d %3d %3d   %3d %3d %3d\n\n", 
        cube.getVoxel(0, 0, 0), cube.getVoxel(1, 0, 0), cube.getVoxel(2, 0, 0), 
        cube.getVoxel(0, 0, 1), cube.getVoxel(1, 0, 1), cube.getVoxel(2, 0, 1), 
        cube.getVoxel(0, 0, 2), cube.getVoxel(1, 0, 2), cube.getVoxel(2, 0, 2));

    wrefresh(cubeWindow);
}
