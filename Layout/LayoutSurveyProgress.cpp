#include "LayoutSurveyProgress.h"

LayoutSurveyProgress::LayoutSurveyProgress(DOGS102* lcd)
  : Layout(lcd),
    _lMsg1(5, 2, "Survey"),
    _lMsg2(7, 3, "in"),
    _lMsg3(4, 4, "Progress"),
    _fCountdownLabel(0, 6, 17),
    _fCountdown(0, 7, 9)
{}

LayoutSurveyProgress::~LayoutSurveyProgress() {}

void LayoutSurveyProgress::display() {
    clear();
    startUpdate();

    writeLabel(_lMsg1);
    writeLabel(_lMsg2);
    writeLabel(_lMsg3);

    endUpdate();
}

void LayoutSurveyProgress::updateCountdown(uint32_t seconds) {
    char buf[16];
    size_t size;

    writeField(_fCountdownLabel, "No Free Channel", true);
    size = snprintf(buf, sizeof(buf), "%lu s", seconds);
    writeField(_fCountdown, buf, size, true);
}

