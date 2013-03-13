#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>

#include "Main.h"
#include "UI.h"
#include "GPIO_Access.h"
#include "Cube.h"
#include "CubeConstants.h"
#include "Tools.h"
#include "Timing.h"
#include "AnimationCubePulse.h"
#include "AnimationRunningLight.h"
#include "AnimationSpotControl.h"
#include "AnimationPause.h"
#include "AnimationRandomPulse.h"
#include "AnimationMovingWall.h"

using namespace std;

bool Main::finished = false;
int Main::exitCode = 0;

Main::Main() {
    animation = NULL;

    UI::init();
}

bool Main::isFinished() {
    return finished;
}

Main::~Main() {
    if(animation != NULL)
        delete animation;

    UI::shutdown();
}

int Main::run(int argc, char **argv) {
    long target_hz = 5000L;
    
    // first argument is target refresh rate
    if(argc > 1) {
        target_hz = atol(argv[1]);
    }
    
    bool lowHzMode = (target_hz < 256);
    
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
    gettimeofday(&last_second, 0);
    int last_count = 0;
    
    // initialize timing status
    timing.startCycles();
    
    // set up initial Animation
    animation = new AnimationCubePulse();
    uint64_t nextAnimFrame = timing.getNow();
        
    for(int count=0;isFinished() != true;++count) {        
        // get next animation step if due
        if(timing.isTimeReached(nextAnimFrame))
        {
            nextAnimFrame = timing.getFutureTime(animation->getFrameMs());
            animation->setNextFrame(cube);
            
            UI::refreshCube(cube);
            
            doInputCheck();;
        }
        
        // refresh LED cache for next refresh
        if(!lowHzMode) 
            cube.refreshCubeForBamPosition(count%256);
        
        // clock out layer
        long usIdle = 0;
        for(int layer = 0; layer < CUBE_SIZE_LAYERS; ++layer) {            
            Tools::clear_cube(gpio);
            
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
            for(int y = 0; y < CUBE_SIZE_Y; ++y) {
                for(int x = 0; x < CUBE_SIZE_X; ++x) {
                    if(lowHzMode)
                        cube.getVoxel(x, y, layer)?gpio.setPort(SD1) : gpio.clearPort(SD1);                    
                    else
                        cube.getVoxelStatus(x, y, layer)?gpio.setPort(SD1) : gpio.clearPort(SD1);
                    Tools::clock_it(gpio);
                }
            }
            
            // LE on
            gpio.setPort(LATCH);
            
            // delay until next period in next layer
            usIdle += timing.waitForNextCycle();

	    // clear Cube after each layer was displayed
            Tools::clear_cube(gpio);
        }
        
        // performance measurement
        gettimeofday(&current_time, 0);
        if(current_time.tv_sec > last_second.tv_sec) {
            UI::setStatusHz(count-last_count);
            UI::setStatusAnimationName(animation->getAnimationName());
            UI::refreshStatus();
            
            char message[200];
            sprintf(message, "%ld uSec idle in last frame", usIdle);
            UI::addStatusMessage(message);
            
            last_second = current_time;
            last_count = count;            
        }                
    }
    
    return exitCode;
}

void Main::finish(int sig) {
    finished = true;
    exitCode = sig;    
}

void Main::doInputCheck() {
    // check for keyboard input
    int ch = UI::getKey();
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
        case KEY_F(1): 
            delete animation;
            animation = new AnimationCubePulse();
            break;
        case KEY_F(2): 
            delete animation;
            animation = new AnimationRunningLight();
            break;
        case KEY_F(3): 
            delete animation;
            animation = new AnimationSpotControl();
            break;
        case KEY_F(4): 
            delete animation;
            animation = new AnimationRandomPulse();
            break;
        case KEY_F(5): 
            delete animation;
            animation = new AnimationMovingWall();
            break;

        default:
            // send to animation
            animation->handleKeyPress(ch);
            break;
    }
}
