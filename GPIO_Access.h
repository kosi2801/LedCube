/***
 * Class which provides GPIO access via direct memory mapping.
 ***/

enum GPIO_Port {
    GPIO_0 = 0,
    GPIO_1 = 1, 
    GPIO_4 = 4,
    GPIO_7 = 7,
    GPIO_8 = 8,
    GPIO_9 = 9,
    GPIO_10 = 10,
    GPIO_11 = 11, 
    GPIO_14 = 14,
    GPIO_15 = 15,
    GPIO_17 = 17,
    GPIO_18 = 18, 
    GPIO_21 = 21,
    GPIO_22 = 22,
    GPIO_23 = 23,
    GPIO_24 = 24,
    GPIO_25 = 25,
};

enum GPIO_Mode {
    GPIO_IN,
    GPIO_OUT,
};

class GPIO_Access {
    public:
     GPIO_Access();
     ~GPIO_Access();

     void setMode(GPIO_Port port, GPIO_Mode mode);
     void setPort(GPIO_Port port);
     void clearPort(GPIO_Port port);

    private:
     volatile unsigned *gpio;
     void initGpioAccess();    
};

