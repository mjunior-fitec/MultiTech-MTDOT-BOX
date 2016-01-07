#include "LayoutConfig.h"

LayoutConfig::LayoutConfig(DOGS102* lcd)
  : Layout(lcd),
    _lMode(0, 0, "Configuration"),
    _fHeader(0, 1, 17),
    _fMsg1(0, 3, 17),
    _fMsg2(0, 4, 17),
    _fMsg3(0, 5, 17)
{
    _timer.start();
}

LayoutConfig::~LayoutConfig() {}

void LayoutConfig::display() {
    clear();
    startUpdate();
    writeLabel(_lMode);
    endUpdate();
    update1of3();
}

void LayoutConfig::roll(){
    if(_timer.read_ms() < _duration){
        return;
    }
    if(_screen == 3){
        _screen = 1;
    }
    else
        _screen++;
    
    switch(_screen){
        case 1:
            update1of3();
            break;
        case 2:
            update2of3();
            break;
        case 3:
            update3of3();
            break;
    }
}

void LayoutConfig::update1of3() {
    _screen = 1;
    _timer.reset();
    _duration = 4000;
    writeField(_fHeader, string("1 of 3"), true);
    writeField(_fMsg1, string("Connect to MTMDK"), true);
    writeField(_fMsg2, string("using the 8 pin"), true);
    writeField(_fMsg3, string("ribbon cable."), true);
}

void LayoutConfig::update2of3() {
    _timer.reset();
    _duration = 3000;
    writeField(_fHeader, string("2 of 3"), true);
    writeField(_fMsg1, string("Plug MTMDK into a"), true);
    writeField(_fMsg2, string("PC."), true);
    writeField(_fMsg3, string(""), true);
}

void LayoutConfig::update3of3() {
    _timer.reset();
    _duration = 5000;
    writeField(_fHeader, string("3 of 3"), true);
    writeField(_fMsg1, string("Run com s/w on"), true);
    writeField(_fMsg2, string("new TTY/COM at"), true);
    writeField(_fMsg3, string("115200 bps."), true);
}
