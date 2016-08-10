/* /* Copyright (c) <2016> <MultiTech Systems>, MIT License
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

#include "LayoutJoin.h"

LayoutJoin::LayoutJoin(DOGS102* lcd, uint8_t band)
  : Layout(lcd),
    _lId(0, 1, "NI="),
    _lKey(0, 2, "NK="),
    _lFsb(0, 3, "FSB="),
    _lRate(0, 5, "DR="),
    _lPower(6, 5, "P="),
    _lAttempt(11, 5, "A="),
    _fStatus(0, 0, 17),
    _fId(3, 1, 14),
    _fKey(3, 2, 14),
    _fFsb(4, 3, 2),
    _fRate(3, 5, 2),
    _fPower(8, 5, 2),
    _fAttempt(13, 5, 4),
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
    writeLabel(_lAttempt);

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
    writeField(_fRate, rate, true);
}

void LayoutJoin::updatePower(uint32_t power) {
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", power);
    writeField(_fPower, buf, size, true);
}

void LayoutJoin::updateAttempt(uint32_t attempt) {
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", attempt);
    writeField(_fAttempt, buf, size, true);
}

void LayoutJoin::updateStatus(std::string status) {
    writeField(_fStatus, status, true);
}

void LayoutJoin::updateCountdown(uint32_t seconds) {
    char buf[16];
    size_t size;

    // make sure the string version is used
    writeField(_fCountdownLabel, string("No Free Channel"), true);
    size = snprintf(buf, sizeof(buf), "%lu s", seconds);
    writeField(_fCountdown, buf, size, true);
}

void LayoutJoin::displayCancel(bool display) {
    std::string str;
    if (display)
        str = "Cancel";
    else
        str = string(17, ' ');

    writeField(_fCancel, str, true);
}

void LayoutJoin::updateJoinFsb(uint8_t band) {
    char buf[8];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%u", band);
    writeField(_fFsb, buf, size, true);
    size = snprintf(buf, sizeof(buf), "     %u", band);
    writeField(_fCancel, buf, size, true);
}

void LayoutJoin::displayEditFsb(string rate, uint32_t power, uint8_t band, string key, string id){    
    clear();
    startUpdate();

    writeLabel(_lId);
    writeLabel(_lKey);
    writeLabel(_lFsb);
    writeLabel(_lRate);
    writeLabel(_lPower);
    writeField(_fCountdown, string("Join"), true);
    updateId(id);
    updateKey(key);
    updateJoinFsb(band);
    updateRate(rate);
    updatePower(power);
    
    endUpdate();  
}
