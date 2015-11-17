#include "LayoutJoin.h"

LayoutJoin::LayoutJoin(DOGS102* lcd, uint8_t band)
  : Layout(lcd),
    _lId(0, 1, "NI="),
    _lKey(0, 2, "NK="),
    _lFsb(0, 3, "FSB="),
    _lRate(0, 5, "DR="),
    _lPower(6, 5, "P="),
    _fStatus(0, 0, 17),
    _fId(3, 1, 14),
    _fKey(3, 2, 14),
    _fFsb(4, 3, 2),
    _fRate(3, 5, 2),
    _fPower(8, 5, 2),
    _fCountdown(0, 7, 9),
    _fCountdownLabel(0, 6, 17),
    _fCancel(11, 7, 6),
    _band(band)
{}

LayoutJoin::~LayoutJoin() {}

void LayoutJoin::display() {
    clear();
    startUpdate();

    writeLabel(_lId);
    writeLabel(_lKey);
    if (_band == mDot::FB_915) {
        writeLabel(_lFsb);
    }
    writeLabel(_lRate);
    writeLabel(_lPower);

    displayCancel();
    
    endUpdate();
}

void LayoutJoin::updateId(std::string id) {
    writeField(_fId, id, true);
}

void LayoutJoin::updateKey(std::string key) {
    writeField(_fKey, key, true);
}

void LayoutJoin::updateFsb(uint8_t band) {
    char buf[8];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%u", band);
    writeField(_fFsb, buf, size, true);
}

void LayoutJoin::updateRate(std::string rate) {
    writeField(_fRate, rate);
}

void LayoutJoin::updatePower(uint32_t power) {
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", power);
    writeField(_fPower, buf, size, true);
}

void LayoutJoin::updateStatus(std::string status) {
    writeField(_fStatus, status, true);
}

void LayoutJoin::updateCountdown(uint32_t seconds) {
    char buf[16];
    size_t size;

    writeField(_fCountdownLabel, "No Free Channel");
    size = snprintf(buf, sizeof(buf), "%lu", seconds);
    writeField(_fPower, buf, size, true);
}

void LayoutJoin::removeCountdown() {
    removeField(_fCountdownLabel);
    removeField(_fCountdown);
}

void LayoutJoin::displayCancel(bool display) {
    if (display)
        writeField(_fCancel, "Cancel", true);
    else
        removeField(_fCancel);
}
