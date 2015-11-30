#include "LayoutSweepProgress.h"

LayoutSweepProgress::LayoutSweepProgress(DOGS102* lcd)
  : Layout(lcd),
    _lSlash(8, 1, "/"),
    _lMsg1(5, 2, "Sweep"),
    _lMsg2(7, 3, "in"),
    _lMsg3(4, 4, "Progress"),
    _fComplete(6, 1, 2),
    _fTotal(9, 1, 2),
    _fCountdownLabel(0, 6, 17),
    _fCountdown(0, 7, 9)
{}

LayoutSweepProgress::~LayoutSweepProgress() {}

void LayoutSweepProgress::display() {
    clear();
    startUpdate();

    writeLabel(_lSlash);
    writeLabel(_lMsg1);
    writeLabel(_lMsg2);
    writeLabel(_lMsg3);

    endUpdate();
}

void LayoutSweepProgress::updateProgress(uint8_t complete, uint8_t total) {
    char buf[8];
    size_t size;

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%2u", complete);
    writeField(_fComplete, buf, size, true);

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%u", total);
    writeField(_fTotal, buf, size, true);
}

void LayoutSweepProgress::updateCountdown(uint32_t seconds) {
    char buf[16];
    size_t size;

    memset(buf, 0, sizeof(buf));
    // for some reason, there's a % character that gets displayed in the last column
    // add the extra spaces to wipe it out
    writeField(_fCountdownLabel, "No Free Channel  ", true);
    size = snprintf(buf, sizeof(buf), "%lu s", seconds);
    writeField(_fCountdown, buf, size, true);
}

