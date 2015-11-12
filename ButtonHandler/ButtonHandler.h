#ifndef __BUTTONHANDLER_H__
#define __BUTTONHANDLER_H__

#include "mbed.h"
#include "rtos.h"

#define buttonSignal (uint32_t)0x01

typedef enum {
    none = 0,
    sw1_press,
    sw1_hold,
    sw2_press
} ButtonEvent;

class ButtonHandler {
    public:
        ButtonHandler(osThreadId main);
        ~ButtonHandler();

        ButtonEvent getButtonEvent();

    private:
        void sw1_fall();
        void sw1_rise();
        void sw2_fall();

        osThreadId _main;
        InterruptIn _sw1;
        InterruptIn _sw2;
        Timer _sw1_timer;
        time_t _sw1_time;
        ButtonEvent _event;
};

#endif
