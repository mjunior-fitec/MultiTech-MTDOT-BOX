#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(osThreadId main)
  : _main(main),
    _sw1(PA_12),
    _sw2(PA_11),
    _sw1_time(0),
    _event(none)
{
    // gpio goes low when push button is pressed
    // fall handler will be the press, rise handler will be the release
    _sw1.fall(this, &ButtonHandler::sw1_fall);
    _sw1.rise(this, &ButtonHandler::sw1_rise);
    // need to set mode to PullUp after attaching handlers - won't work otherwise
    _sw1.mode(PullUp);

    _sw2.fall(this, &ButtonHandler::sw2_fall);
    _sw2.mode(PullUp);
}

ButtonEvent ButtonHandler::getButtonEvent() {
    ButtonEvent event = _event;
    _event = none;
    return event;
}

void ButtonHandler::sw1_rise() {
    _sw1_timer.stop();
    _sw1_time = _sw1_timer.read_ms();

    if (_sw1_time > 10) {
        _event = (_sw1_time > 500) ? sw1_hold : sw1_press;
        osSignalSet(_main, buttonSignal);
    }
}

void ButtonHandler::sw1_fall() {
    _sw1_timer.reset();
    _sw1_timer.start();
}

void ButtonHandler::sw2_fall() {
    _event = sw2_press;
    osSignalSet(_main, buttonSignal);
}
