#include "LayoutFile.h"

LayoutFile::LayoutFile(DOGS102* lcd)
  : Layout(lcd),
    _lMsg1(0, 0, "Erase or Append"),
    _lMsg2(0, 1, "to Existing"),
    _lMsg3(0, 2, "Survey Data File?"),
    _lIns1(0, 4, "Hold SW1 any time"),
    _lIns2(0, 5, "for Main Menu"),
    _lSw1(11, 7, "Append"),
    _lSw2(0, 7, "Erase")
{}

LayoutFile::~LayoutFile() {}

void LayoutFile::display() {
    clear();
    startUpdate();

    writeLabel(_lMsg1);
    writeLabel(_lMsg2);
    writeLabel(_lMsg3);
    writeLabel(_lIns1);
    writeLabel(_lIns2);
    writeLabel(_lSw1);
    writeLabel(_lSw2);

    endUpdate();
}

