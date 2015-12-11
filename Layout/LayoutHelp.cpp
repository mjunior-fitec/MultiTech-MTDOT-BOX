#include "LayoutHelp.h"

LayoutHelp::LayoutHelp(DOGS102* lcd)
  : Layout(lcd),
    _lIns1(0, 4, "Hold SW1 any time"),
    _lIns2(0, 5, "for Main Menu"),
    _fMode(0, 0, 17),
    _fDesc(0, 1, 17),
    _fMsg(0, 6, 17),
    _fSw1(9, 7, 8),
    _fSw2(0, 7, 8)
{}

LayoutHelp::~LayoutHelp() {}

void LayoutHelp::display() {
    clear();
    startUpdate();

    writeLabel(_lIns1);
    writeLabel(_lIns2);

    endUpdate();
}

void LayoutHelp::updateMode(std::string mode) {
    writeField(_fMode, mode, true);
}

void LayoutHelp::updateDescription(std::string description) {
    writeField(_fDesc, description, true);
}

void LayoutHelp::updateMsg(std::string msg) {
    writeField(_fMsg, msg, true);
}

void LayoutHelp::updateSw1(std::string s) {
    writeField(_fSw1, s, true);
}

void LayoutHelp::updateSw2(std::string s) {
    writeField(_fSw2, s, true);
}

