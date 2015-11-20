#include "LayoutSurveyFailure.h"

LayoutSurveyFailure::LayoutSurveyFailure(DOGS102* lcd)
  : Layout(lcd),
    _lTitle(0, 2, "Survey Failed"),
    _lId(0, 0, "ID"),
    _lDr(8, 0, "DR"),
    _lPwr(13, 0, "P"),
    _lSw1(12, 7, "Power"),
    _lSw2(0, 7, "Survey"),
    _fId(2, 0, 5),
    _fDr(10, 0, 2),
    _fPwr(14, 0, 2),
    _fMsg1(0, 4, 17),
    _fMsg2(0, 5, 17),
    _fInfo(0, 6, 17)
{}

LayoutSurveyFailure::~LayoutSurveyFailure() {}

void LayoutSurveyFailure::display() {
    clear();
    startUpdate();

    writeLabel(_lTitle);
    writeLabel(_lId);
    writeLabel(_lDr);
    writeLabel(_lPwr);
    writeLabel(_lSw1);
    writeLabel(_lSw2);

    endUpdate();
}

void LayoutSurveyFailure::updateId(uint32_t id) {
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", id);
    writeField(_fId, buf, size, true);
}

void LayoutSurveyFailure::updateRate(std::string rate) {
    writeField(_fDr, rate, true);
}

void LayoutSurveyFailure::updatePower(uint32_t power) {
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", power);
    writeField(_fPwr, buf, size, true);
}

void LayoutSurveyFailure::updateInfo(std::string info) {
    writeField(_fInfo, info, true);
}

