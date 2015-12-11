#include "LayoutSweepProgress.h"

LayoutSweepProgress::LayoutSweepProgress(DOGS102* lcd)
  : Layout(lcd),
    _lMsg1(6, 2, "Sweep"),
    _lMsg2(7, 3, "in"),
    _lMsg3(4, 4, "Progress"),
    _fProgress(6, 1, 5),
    _fCountdownLabel(0, 6, 17),
    _fCountdown(0, 7, 9)
{}

LayoutSweepProgress::~LayoutSweepProgress() {}

void LayoutSweepProgress::display() {
    clear();
    startUpdate();

    writeLabel(_lMsg1);
    writeLabel(_lMsg2);
    writeLabel(_lMsg3);

    endUpdate();
}

void LayoutSweepProgress::updateProgress(uint8_t complete, uint8_t total) {
    char buf[8];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%u/%u", complete, total);
    writeField(_fProgress, buf, size, true);
}

void LayoutSweepProgress::updateCountdown(uint32_t seconds) {
    char buf[16];
    size_t size;

    // make sure the string version is used
    writeField(_fCountdownLabel, string("No Free Channel"), true);
    size = snprintf(buf, sizeof(buf), "%lu s", seconds);
    writeField(_fCountdown, buf, size, true);
}

