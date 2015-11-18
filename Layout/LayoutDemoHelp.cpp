#include "LayoutDemoHelp.h"

LayoutDemoHelp::LayoutDemoHelp(DOGS102* lcd)
  : Layout(lcd),
    _lMode(0, 0, "LoRa Demo"),
    _lDesc(0, 1, "Select TX Method"),
    _lIns1(0, 4, "Hold SW1 any time"),
    _lIns2(0, 5, "for Main Menu"),
    _lSw1(10, 7, "Trigger"),
    _lSw2(0, 7, "Interval")
{}

LayoutDemoHelp::~LayoutDemoHelp() {}

void LayoutDemoHelp::display() {
    clear();
    startUpdate();

    writeLabel(_lMode);
    writeLabel(_lDesc);
    writeLabel(_lIns1);
    writeLabel(_lIns2);
    writeLabel(_lSw1);
    writeLabel(_lSw2);

    endUpdate();
}

