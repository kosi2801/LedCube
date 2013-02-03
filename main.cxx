#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>

#include <ncurses.h>
#include <signal.h>

#include "GPIO_Access.h"
#include "Cube.h"
#include "CubeConstants.h"
#include "Tools.h"
#include "Timing.h"
#include "AnimationCubePulse.h"
#include "AnimationRunningLight.h"
#include "AnimationPause.h"


using namespace std;

static void finish(int sig);
static void doInputCheck();
static Animation* animation;

int main(int argc, char **argv)
{
    long target_hz = 5000L;
    
    // first argument is target refresh rate
    if(argc > 1) {
        target_hz = atol(argv[1]);
    }
    
    Timing timing(target_hz);
    GPIO_Access gpio;
    
    gpio.setMode(CLK, GPIO_OUT);
    gpio.setMode(LATCH, GPIO_OUT);
    gpio.setMode(SD1, GPIO_OUT);
    gpio.setMode(SD2, GPIO_OUT);
    
    // LE on, CLK off, SD1 off
    gpio.setPort(LATCH);
    gpio.clearPort(CLK);
    gpio.clearPort(SD1);    
    gpio.clearPort(SD2);    
    
    // the cube status
    Cube cube;
    
    // required for performance measurement
    timeval current_time, last_second;    
    int last_count = 0;
    
    // initialize timing status
    timing.startCycles();
    
    // set up initial Animation
    animation = new AnimationCubePulse();
    timeval nextAnimFrame = timing.getNow();
    
    initscr();
    signal(SIGINT, finish);
    nodelay(stdscr, TRUE);
    cbreak();
    noecho();
    clear();
    keypad(stdscr, TRUE);
    
    for(int count=0;;++count) {        
        // get next animation step if due
        if(timing.isTimeReached(nextAnimFrame))
        {
            //printf("Time reached \n\r");
            nextAnimFrame = timing.getFutureTime(animation->getFrameMs());
            animation->setNextFrame(cube);
            
            cube.printStatus();
            refresh();
            
            doInputCheck();;
        }
        
        // clock out layer
        for(int layer = 0; layer < CUBE_SIZE_LAYERS; ++layer) {            
//            Tools::clear_cube(gpio);
            
            // LE off
            gpio.clearPort(LATCH);
        
            // 4 low pins padding
            gpio.clearPort(SD1);
            for(int i=0; i<4; ++i) {
                Tools::clock_it(gpio);
            }
            
            // 3 layer pins
            uint layer_bit = 1 << layer;
            layer_bit&(1<<0) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
            Tools::clock_it(gpio);
            layer_bit&(1<<1) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
            Tools::clock_it(gpio);
            layer_bit&(1<<2) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
            Tools::clock_it(gpio);

            // 9 cube pins
            // if target Hz>255 enable BAM
            uint cube_bits;
            if(target_hz > 255)
            {
                cube_bits = cube.getLayerBamBitfield(layer, count%256);
            } else {
                cube_bits = cube.getLayerBitfield(layer);
            }
            
            for(int i=0; i<9; ++i) {
                (cube_bits & (1<<i)) ? gpio.setPort(SD1) : gpio.clearPort(SD1);
                Tools::clock_it(gpio);
            }
            
            // LE on
            gpio.setPort(LATCH);
            
            // delay until next period in next layer
            timing.waitForNextCycle();
        }
        
        // performance measurement
        gettimeofday(&current_time, 0);
        if(current_time.tv_sec > last_second.tv_sec) {
            mvprintw(0,0,"%d Hz in the last second", count-last_count);
            mvprintw(8,0,"Current animation: %s          ", animation->getAnimationName());
            refresh();
            
            last_second = current_time;
            last_count = count;            
        }                
    }
    
    finish(0);
} 

static void doInputCheck()
{
        // check for keyboard input
        int ch = getch();
        switch (ch) {
            case ERR:
                // no action on error
                break;
            case ' ': 
            {
                // pause on/off on SPACE
                //check if already paused by checking real class. Ugly but we're not attending a contest...
                AnimationPause* pause = dynamic_cast<AnimationPause*>(animation);
                if(pause != NULL) {
                    // already paused, restore old anim and remove pause
                    animation = pause->getPausedAnimation();
                    pause->setPausedAnimation(NULL); // don't forget or it will delete our old animation when destructed
                    delete pause;
                } else {
                    animation = new AnimationPause(animation);
                }                    
                break;
            }
            case '1': 
                delete animation;
                animation = new AnimationCubePulse();
                break;
            case '2': 
                delete animation;
                animation = new AnimationRunningLight();
                break;

            default:
                break;
        }
        if (ch == ERR) {
            ; // nothing
        } else {
            switch (ch) {
                case KEY_ENTER: 
                    finish(0);
                    break;
            }
        }
}

static void finish(int sig)
{
    delete animation;
    endwin();
    exit(0);
}
