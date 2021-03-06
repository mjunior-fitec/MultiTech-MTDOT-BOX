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

#include "ButtonHandler.h"

#define signal (int32_t)0xA0

typedef enum {
    b_none = 0,
    b_sw1_fall,
    b_sw1_rise,
    b_sw2_fall,
    b_sw2_rise
} InternalButtonEvent;

InternalButtonEvent event = b_none;
bool check_sw1 = false;

void b_worker(void const* argument) {
    ButtonHandler* b = (ButtonHandler*)argument;
    osEvent e;

    while (true) {
        e = Thread::signal_wait(signal, 250);
        if (e.status == osEventSignal) {
            switch (event) {
                case b_sw1_fall:
                    if (! b->_sw1_running) {
                        check_sw1 = true;
                        b->_sw1_running = true;
                        b->_sw1_timer.reset();
                        b->_sw1_timer.start();
                    }
                    break;
                
                case b_sw1_rise:
                    if (b->_sw1_running) {
                        check_sw1 = false;
                        b->_sw1_running = false;
                        b->_sw1_timer.stop();
                        b->_sw1_time = b->_sw1_timer.read_ms();

                        if (b->_sw1_time > b->_debounce_time) {
                            b->_event = ButtonHandler::sw1_press;
                            osSignalSet(b->_main, buttonSignal);
                        }
                    }
                    break;

                case b_sw2_fall:
                    if (! b->_sw2_running) {
                        b->_sw2_running = true;
                        b->_sw2_timer.reset();
                        b->_sw2_timer.start();
                    }
                    break;

                case b_sw2_rise:
                    if (b->_sw2_running) {
                        b->_sw2_running = false;
                        b->_sw2_timer.stop();
                        b->_sw2_time = b->_sw2_timer.read_ms();

                        if (b->_sw2_time > b->_debounce_time) {
                            b->_event = ButtonHandler::sw2_press;
                            osSignalSet(b->_main, buttonSignal);
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        if (check_sw1) {
            if (b->_sw1_timer.read_ms() > b->_hold_threshold) {
                check_sw1 = false;
                b->_sw1_running = false;
                b->_sw1_timer.stop();
                b->_event = ButtonHandler::sw1_hold;
                osSignalSet(b->_main, buttonSignal);
            }
        }
    }
}

ButtonHandler::ButtonHandler(osThreadId main)
  : _main(main),
    _thread(b_worker, (void*)this),
    _sw1(PA_12),
    _sw2(PA_11),
    _sw1_time(0),
    _sw2_time(0),
    _sw1_running(false),
    _sw2_running(false),
    _event(none),
    _debounce_time(20),
    _hold_threshold(500)
{
    // fall handler called on press, rise handler called on release
    _sw1.fall(this, &ButtonHandler::sw1_fall);
    _sw1.rise(this, &ButtonHandler::sw1_rise);
    // need to set mode to PullUp after attaching handlers - default is PullNone (see PinNames.h)
    _sw1.mode(PullUp);

    _sw2.fall(this, &ButtonHandler::sw2_fall);
    _sw2.rise(this, &ButtonHandler::sw2_rise);
    _sw2.mode(PullUp);
}

ButtonHandler::ButtonEvent ButtonHandler::getButtonEvent() {
    ButtonEvent event = _event;
    _event = none;
    return event;
}

void ButtonHandler::sw1_fall() {
    event = b_sw1_fall;
    _thread.signal_set(signal);
    _thread.signal_clr(signal);
}

void ButtonHandler::sw1_rise() {
    event = b_sw1_rise;
    _thread.signal_set(signal);
    _thread.signal_clr(signal);
}

void ButtonHandler::sw2_fall() {
    event = b_sw2_fall;
    _thread.signal_set(signal);
    _thread.signal_clr(signal);
}

void ButtonHandler::sw2_rise() {
    event = b_sw2_rise;
    _thread.signal_set(signal);
    _thread.signal_clr(signal);
}

