#define BCM2835_PERI_BASE 0x20000000
#define GPIO_BASE (BCM2835_PERI_BASE + 0x200000) /* GPIO controller */
#define ST_BASE (BCM2835_PERI_BASE + 0x3000) /* System timer */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

#include "GPIO_Access.h"

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define ST_CLO *(st+1)  // lower 32 bits of running system timer    10^6 = 1sec
#define ST_CHI *(st+2)  // higher 32 bits of running system timer

GPIO_Access::GPIO_Access()
{
    initGpioAccess();
}

GPIO_Access::~GPIO_Access()
{
    ;
}

void GPIO_Access::setMode(GPIO_Port port, GPIO_Mode mode)
{
    INP_GPIO(port); // must use INP_GPIO before we can use OUT_GPIO or SET_GPIO_ALT
    
    if(GPIO_OUT == mode) 
    {
        OUT_GPIO(port);
    }
}

void GPIO_Access::setPort(GPIO_Port port)
{
    GPIO_SET = 1<<port;
}

void GPIO_Access::clearPort(GPIO_Port port)
{
    GPIO_CLR = 1 << port;
}

uint64_t GPIO_Access::getSystemTimer()
{
    uint64_t stime = ST_CHI;
    stime <<= 32;
    stime += ST_CLO;
    return stime;
}

void GPIO_Access::initGpioAccess()
{
    int  mem_fd;
    volatile uint32_t *mem_map;
    
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   mem_map = (volatile uint32_t *)mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   if (mem_map == MAP_FAILED) {
      printf("mmap gpio error: %s\n", strerror(errno));
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile uint32_t *)mem_map;
   
   /* mmap System Timer */
   mem_map = (volatile uint32_t *)mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ,// Only reading from mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      ST_BASE         //Offset to System Timer peripheral
   );

   if (mem_map == MAP_FAILED) {
      printf("mmap st error: %s\n", strerror(errno));
      exit(-1);
   }

   // Always use volatile pointer!
   st = (volatile uint32_t *)mem_map;
   
   close(mem_fd); //No need to keep mem_fd open after mmap
}

