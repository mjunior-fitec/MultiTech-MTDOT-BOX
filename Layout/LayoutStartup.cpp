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

#include "LayoutStartup.h"
#include "MultiTech_Logo.h"
#include "version.h"
#include "MTSText.h"

LayoutStartup::LayoutStartup(DOGS102* lcd, mDot* dot)
  : Layout(lcd),
    _dot(dot),
    _lName(0, 3, "MTDOT-BOX/EVB"),
    _lInfo(0, 4, "Factory Firmware"),
    _lVersion(0, 5, "Version"),
    _fName(14, 3, 3),
    _fVersion(8, 5, 9),
    _fId(0, 7, 17),
    _fGps(0, 6, 17),
    _iLogo(0, 0, MultiTech_Logo)
{}

LayoutStartup::~LayoutStartup() {}

void LayoutStartup::display() {
    std::string version = MTDOT_BOX_VERSION;
    std::string id = mts::Text::bin2hexString(_dot->getDeviceId());

    clear();
    startUpdate();

    writeImage(_iLogo);
    writeLabel(_lName);
    writeLabel(_lInfo);
    writeLabel(_lVersion);
    writeField(_fName, mDot::FrequencyBandStr(_dot->getFrequencyBand()).substr(3));
    writeField(_fVersion, version);
    writeField(_fId, id);

    endUpdate();
}

void LayoutStartup::updateGPS(bool gps) {
    char buf[32];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%sGPS Detected", gps ? "" : "No ");
    writeField(_fGps, buf, size);
}

