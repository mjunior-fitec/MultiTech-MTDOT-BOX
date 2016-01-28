/* Copyright (c) <2016> <MultiTech Systems>, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
    _fGpsLat(0, 4, 17),
    _fGpsLon(0, 3, 17),
    _fGpsTime(0, 5, 17),
    _fInfo(0, 6, 17),
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

void LayoutSurveyFailure::updateGpsLatitude(GPSPARSER::latitude lat) {
    char buf[32];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%d %d %d.%03d %c",
        abs(lat.degrees),
        lat.minutes,
        (lat.seconds * 6) / 1000,
        (lat.seconds * 6) % 1000,
        (lat.degrees > 0) ? 'N' : 'S');
    writeField(_fGpsLat, buf, size, true);
}

void LayoutSurveyFailure::updateGpsLatitude(std::string msg) {
    writeField(_fGpsLat, msg, true);
}

void LayoutSurveyFailure::updateGpsLongitude(GPSPARSER::longitude lon) {
    char buf[32];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%d %d %d.%03d %c",
        abs(lon.degrees),
        lon.minutes,
        (lon.seconds * 6) / 1000,
        (lon.seconds * 6) % 1000,
        (lon.degrees > 0) ? 'E' : 'W');
    writeField(_fGpsLon, buf, size, true);
}

void LayoutSurveyFailure::updateGpsTime(struct tm time) {
    char buf[32];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%02d:%02d %02d/%02d/%04d",
        time.tm_hour,
        time.tm_min,
        time.tm_mon + 1,
        time.tm_mday,
        time.tm_year + 1900);
    writeField(_fGpsTime, buf, size, true);
}

void LayoutSurveyFailure::updateInfo(std::string info) {
    writeField(_fInfo, info, true);
}

void LayoutSurveyFailure::updateSw1(std::string sw1) {
    writeField(_fSw1, sw1, true);
}

void LayoutSurveyFailure::updateSw2(std::string sw2) {
    writeField(_fSw2, sw2, true);
}

void LayoutSurveyFailure::updatePassFail(uint8_t pass, uint8_t fail) {
    char buf[32];
    size_t size;

    size = snprintf(buf, sizeof(buf), "Pass %u Fail %u", pass, fail);
    writeField(_fGpsTime, buf, size, true);
}

