#include "LayoutConfirm.h"

LayoutConfirm::LayoutConfirm(DOGS102* lcd)
  : Layout(lcd),
    _lMsg1(0, 0, "Are You Sure You"),
    _lMsg2(0, 1, "Want to Erase It?"),
    _lIns1(0, 4, "Hold SW1 any time"),
    _lIns2(0, 5, "for Main Menu"),
    _lSw1(14, 7, "No"),
    _lSw2(1, 7, "Yes")
{}

LayoutConfirm::~LayoutConfirm() {}

void LayoutConfirm::display() {
    clear();
    startUpdate();

    writeLabel(_lMsg1);
    writeLabel(_lMsg2);
    writeLabel(_lIns1);
    writeLabel(_lIns2);
    writeLabel(_lSw1);
    writeLabel(_lSw2);

    endUpdate();
}

