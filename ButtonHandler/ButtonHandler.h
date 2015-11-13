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

        void sw1_fall();
        void sw1_rise();
        void sw2_fall();
        void sw2_rise();

        osThreadId _main;
        Thread _thread;
        InterruptIn _sw1;
        InterruptIn _sw2;
        Timer _sw1_timer;
        Timer _sw2_timer;
        time_t _sw1_time;
        time_t _sw2_time;
        bool _sw1_running;
        bool _sw2_running;
        ButtonEvent _event;
        time_t _debounce_time;
        time_t _hold_threshold;
};

#endif
