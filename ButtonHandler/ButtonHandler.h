/* Copyright (c) <2016> <MultiTech Systems>, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __BUTTONHANDLER_H__
#define __BUTTONHANDLER_H__

#include "mbed.h"
#include "rtos.h"

#define buttonSignal (int32_t)0x01

class ButtonHandler {
    public:
        typedef enum {
            none = 0,
            sw1_press,
            sw1_hold,
            sw2_press
        } ButtonEvent;

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
