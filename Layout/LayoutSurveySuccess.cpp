#include "LayoutSurveySuccess.h"

LayoutSurveySuccess::LayoutSurveySuccess(DOGS102* lcd)
  : Layout(lcd),
    _lId(0, 0, "ID"),
    _lDr(8, 0, "DR"),
    _lPwr(13, 0, "P"),
    _lUp(0, 1, "UP"),
    _lDown(0, 2, "DWN"),
    _lSw1(12, 7, "Power"),
    _fId(2, 0, 5),
    _fDr(10, 0, 2),
    _fPwr(14, 0, 2),
    _fUpRssi(3, 1, 7),
    _fUpSnr(11, 1, 5),
    _fDownRssi(4, 2, 7),
    _fDownSnr(12, 2, 5),
    _fGpsLat(0, 4, 17),
    _fGpsLon(0, 3, 17),
    _fGpsTime(0, 5, 17),
    _fInfo(0, 6, 17),
    _fSw2(0, 7, 8)
{}

LayoutSurveySuccess::~LayoutSurveySuccess() {}

void LayoutSurveySuccess::display() {
    clear();
    startUpdate();

    writeLabel(_lId);
    writeLabel(_lDr);
    writeLabel(_lPwr);
    writeLabel(_lUp);
    writeLabel(_lDown);
    writeLabel(_lSw1);

    endUpdate();
}

void LayoutSurveySuccess::updateId(uint32_t id) {
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", id);
    writeField(_fId, buf, size, true);
}

void LayoutSurveySuccess::updateRate(std::string rate) {
    writeField(_fDr, rate, true);
}

void LayoutSurveySuccess::updatePower(uint32_t power) {
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", power);
    writeField(_fPwr, buf, size, true);
}

void LayoutSurveySuccess::updateStats(LoRaHandler::LoRaPing ping) {
    char buf[16];
    size_t size;

    startUpdate();

    size = snprintf(buf, sizeof(buf), "%3d dbm", ping.up.rssi);
    writeField(_fUpRssi, buf, size);

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%2d.%1d", ping.up.snr / 10, abs(ping.up.snr) % 10);
    writeField(_fUpSnr, buf, size);

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%3d dbm", ping.down.rssi);
    writeField(_fDownRssi, buf, size);

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%2d.%1d", ping.down.snr / 4, abs(ping.up.snr) % 10 * 25);
    writeField(_fDownSnr, buf, size);

    endUpdate();
}

void LayoutSurveySuccess::updateGpsLatitude(GPSPARSER::latitude lat) {
}

void LayoutSurveySuccess::updateGpsLongitude(GPSPARSER::longitude lon) {
}

void LayoutSurveySuccess::updateGpsTime(struct tm time) {
}

void LayoutSurveySuccess::updateInfo(std::string info) {
    writeField(_fInfo, info, true);
}

void LayoutSurveySuccess::updateSw2(std::string sw2) {
    writeField(_fSw2, sw2, true);
}

