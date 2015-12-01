#include "LayoutSurveyFailure.h"

LayoutSurveyFailure::LayoutSurveyFailure(DOGS102* lcd)
  : Layout(lcd),
    _lTitle(0, 1, "Survey Failed"),
    _lId(0, 0, "ID"),
    _lDr(8, 0, "DR"),
    _lPwr(13, 0, "P"),
    _fId(2, 0, 5),
    _fDr(10, 0, 2),
    _fPwr(14, 0, 2),
    _fMsg1(0, 2, 17),
    _fMsg2(0, 3, 17),
    _fInfo1(0, 5, 17),
    _fInfo2(0, 6, 17),
    _fSw1(9, 7, 8),
    _fSw2(0, 7, 8)
{}

LayoutSurveyFailure::~LayoutSurveyFailure() {}

void LayoutSurveyFailure::display() {
    clear();
    startUpdate();

    writeLabel(_lTitle);
    writeLabel(_lId);
    writeLabel(_lDr);
    writeLabel(_lPwr);

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

void LayoutSurveyFailure::updateInfo1(std::string info) {
    writeField(_fInfo1, info, true);
}

void LayoutSurveyFailure::updateInfo2(std::string info) {
    writeField(_fInfo2, info, true);
}

void LayoutSurveyFailure::updatePassFail(uint8_t pass, uint8_t fail) {
    char buf[32];
    size_t size;

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "Pass %u Fail %u", pass, fail);
    writeField(_fInfo1, buf, size, true);
}

void LayoutSurveyFailure::updateSw1(std::string sw1) {
    writeField(_fSw1, sw1, true);
}

void LayoutSurveyFailure::updateSw2(std::string sw2) {
    writeField(_fSw2, sw2, true);
}

