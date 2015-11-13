#include "ButtonHandler.h"

#define signal (uint32_t)0x02

typedef enum {
    b_none = 0,
    b_sw1_fall,
    b_sw1_rise,
    b_sw2_fall,
    b_sw2_rise
} InternalButtonEvent;

InternalButtonEvent event = b_none;
bool check_sw1 = false;

void worker(void const* argument) {
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
                            b->_event = sw1_press;
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
                            b->_event = sw2_press;
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
                b->_event = sw1_hold;
                osSignalSet(b->_main, buttonSignal);
            }
        }
    }
}

ButtonHandler::ButtonHandler(osThreadId main)
  : _main(main),
    _thread(worker, (void*)this),
    _sw1(PA_12),
    _sw2(PA_11),
    _sw1_time(0),
    _sw2_time(0),
    _event(none),
    _debounce_time(20),
    _hold_threshold(500)
{
    // fall handler called on press, rise handler called on release
    _sw1.fall(this, &ButtonHandler::sw1_fall);
    _sw1.rise(this, &ButtonHandler::sw1_rise);
    // need to set mode to PullUp after attaching handlers - won't work otherwise
    _sw1.mode(PullUp);

    _sw2.fall(this, &ButtonHandler::sw2_fall);
    _sw2.rise(this, &ButtonHandler::sw2_rise);
    _sw2.mode(PullUp);
}

ButtonEvent ButtonHandler::getButtonEvent() {
    ButtonEvent event = _event;
    _event = none;
    return event;
}

void ButtonHandler::sw1_fall() {
    event = b_sw1_fall;
    _thread.signal_set(signal);
}

void ButtonHandler::sw1_rise() {
    event = b_sw1_rise;
    _thread.signal_set(signal);
}

void ButtonHandler::sw2_fall() {
    event = b_sw2_fall;
    _thread.signal_set(signal);
}

void ButtonHandler::sw2_rise() {
    event = b_sw2_rise;
    _thread.signal_set(signal);
}

