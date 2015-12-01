#include "LayoutSweepComplete.h"

LayoutSweepComplete::LayoutSweepComplete(DOGS102* lcd)
  : Layout(lcd),
    _lId(0, 0, "ID"),
    _lInfo(0, 1, "Sweep Finished"),
    _lPass(0, 5, "Pass"),
    _lFail(9, 5, "Fail"),
    _lSw2(0, 7, "Sweep"),
    _fId(2, 0, 5),
    _fPass(5, 5, 2),
    _fFail(14, 5, 2)
{}

LayoutSweepComplete::~LayoutSweepComplete() {}

void LayoutSweepComplete::display() {
    clear();
    startUpdate();

    writeLabel(_lId);
    writeLabel(_lInfo);
    writeLabel(_lPass);
    writeLabel(_lFail);
    writeLabel(_lSw2);

    endUpdate();
}

void LayoutSweepComplete::updateId(uint32_t id) {
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", id);
    writeField(_fId, buf, size, true);
}

void LayoutSweepComplete::updatePass(uint8_t pass) {
    char buf[8];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%u", pass);
    writeField(_fPass, buf, size, true);
}

void LayoutSweepComplete::updateFail(uint8_t fail) {
    char buf[8];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%u", fail);
    writeField(_fFail, buf, size, true);
}

