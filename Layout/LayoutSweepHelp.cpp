#include "LayoutSweepHelp.h"

LayoutSweepHelp::LayoutSweepHelp(DOGS102* lcd)
  : Layout(lcd),
    _lMode(0, 0, "Survey Sweep"),
    _lIns1(0, 4, "Hold SW1 any time"),
    _lIns2(0, 5, "for Main Menu"),
    _lSw1(11, 7, "Cancel"),
    _lSw2(0, 7, "Sweep")
{}

LayoutSweepHelp::~LayoutSweepHelp() {}

void LayoutSweepHelp::display() {
    clear();
    startUpdate();

    writeLabel(_lMode);
    writeLabel(_lIns1);
    writeLabel(_lIns2);
    writeLabel(_lSw1);
    writeLabel(_lSw2);

    endUpdate();
}

