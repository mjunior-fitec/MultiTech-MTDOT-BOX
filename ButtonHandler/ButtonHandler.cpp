#include "ButtonHandler.h"
#include "MTSLog.h"

ButtonHandler::ButtonHandler(osThreadId main)
  : _main(main),
    _sw1(PA_12),
    _sw2(PA_11),
    _sw1_time(0),
    _sw2_time(0),
    _event(none),
    _debounce_time(20),
    _hold_threshold(500)
{
    // gpio goes low when push button is pressed
    // fall handler will be the press, rise handler will be the release
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
    if (! _sw1_running) {
        _sw1_running = true;
        _sw1_timer.reset();
        _sw1_timer.start();
    }
}

void ButtonHandler::sw1_rise() {
    if (_sw1_running) {
        _sw1_running = false;
        _sw1_timer.stop();
        _sw1_time = _sw1_timer.read_ms();

        if (_sw1_time > _debounce_time) {
            _event = (_sw1_time > _hold_threshold) ? sw1_hold : sw1_press;
            osSignalSet(_main, buttonSignal);
        }
    }
}

void ButtonHandler::sw2_fall() {
    if (! _sw2_running) {
        _sw2_running = true;
        _sw2_timer.reset();
        _sw2_timer.start();
    }
}

void ButtonHandler::sw2_rise() {
    if (_sw2_running) {
        _sw2_running = false;
        _sw2_timer.stop();
        _sw2_time = _sw2_timer.read_ms();

        if (_sw2_time > _debounce_time) {
            _event = sw2_press;
            osSignalSet(_main, buttonSignal);
        }
    }
}

