#include "LayoutSingleHelp.h"

LayoutSingleHelp::LayoutSingleHelp(DOGS102* lcd)
  : Layout(lcd),
    _lMode(0, 0, "Survey Single"),
    _lIns1(0, 4, "Hold SW1 any time"),
    _lIns2(0, 5, "for Main Menu"),
    _lSw1(11, 7, "DR/PWR"),
    _lSw2(0, 7, "Survey")
{}

LayoutSingleHelp::~LayoutSingleHelp() {}

void LayoutSingleHelp::display() {
    clear();
    startUpdate();

    writeLabel(_lMode);
    writeLabel(_lIns1);
    writeLabel(_lIns2);
    writeLabel(_lSw1);
    writeLabel(_lSw2);

    endUpdate();
}

