#include "LayoutSurveySuccess.h"
#include "MTSLog.h"

LayoutSurveySuccess::LayoutSurveySuccess(DOGS102* lcd)
  : Layout(lcd),
    _lId(0, 0, "ID"),
    _lDr(8, 0, "DR"),
    _lPwr(13, 0, "P"),
    _lUp(0, 1, "UP"),
    _lDown(0, 2, "DWN"),
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
    _fSw1(9, 7, 8),
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
    size = snprintf(buf, sizeof(buf), "%2.1f", (float)ping.up.snr / 10.0);
    writeField(_fUpSnr, buf, size);

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%3d dbm", ping.down.rssi);
    writeField(_fDownRssi, buf, size);

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%2.1f", (float)ping.down.snr / 10.0);
    writeField(_fDownSnr, buf, size);

    endUpdate();
}

void LayoutSurveySuccess::updateGpsLatitude(GPSPARSER::latitude lat) {
    char buf[32];
    size_t size;

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%d %d %d.%03d %c",
        abs(lat.degrees),
        lat.minutes,
        (lat.seconds * 6) / 1000,
        (lat.seconds * 6) % 1000,
        (lat.degrees > 0) ? 'N' : 'S');
    writeField(_fGpsLat, buf, size, true);
}

void LayoutSurveySuccess::updateGpsLatitude(std::string msg) {
    writeField(_fGpsLat, msg, true);
}

void LayoutSurveySuccess::updateGpsLongitude(GPSPARSER::longitude lon) {
    char buf[32];
    size_t size;

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%d %d %d.%03d %c",
        abs(lon.degrees),
        lon.minutes,
        (lon.seconds * 6) / 1000,
        (lon.seconds * 6) % 1000,
        (lon.degrees > 0) ? 'E' : 'W');
    writeField(_fGpsLon, buf, size, true);
}

void LayoutSurveySuccess::updateGpsTime(struct tm time) {
    char buf[32];
    size_t size;

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%02d:%02d %02d/%02d/%04d",
        time.tm_hour,
        time.tm_min,
        time.tm_mon + 1,
        time.tm_mday,
        time.tm_year + 1900);
    writeField(_fGpsTime, buf, size, true);
}

void LayoutSurveySuccess::updateInfo(std::string info) {
    writeField(_fInfo, info, true);
}

void LayoutSurveySuccess::updateSw1(std::string sw1) {
    writeField(_fSw1, sw1, true);
}

void LayoutSurveySuccess::updateSw2(std::string sw2) {
    writeField(_fSw2, sw2, true);
}

void LayoutSurveySuccess::updateCountdown(uint32_t seconds) {
    char buf[32];
    size_t size;

    memset(buf, 0, sizeof(buf));
    // for some reason, there's a % character that gets displayed in the last column
    // add the extra spaces to wipe it out
    writeField(_fInfo, "No Free Channel  ", true);
    size = snprintf(buf, sizeof(buf), "%lu s", seconds);
    writeField(_fSw2, buf, size, true);
}

void LayoutSurveySuccess::updatePassFail(uint8_t pass, uint8_t fail) {
    char buf[32];
    size_t size;

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "Pass %u Fail %u", pass, fail);
    writeField(_fGpsTime, buf, size, true);
}

