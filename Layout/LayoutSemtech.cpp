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

#include "LayoutSemtech.h"


LayoutSemtech::LayoutSemtech(DOGS102* lcd, uint8_t band)
    : Layout(lcd),
      _band(band),
      _lDR(8,0,"DR"),
      _lFSB(0,0,"FSB"),
      _lSend(3,3,"Sending..."),
      _lTemp(8,6,"Temp "),
      _lNoGps(0,4,"No Gps Lock"),
      _lPower(13,0,"P"),
      _lBlank(0,1,"                 "),
      _lNoLink(0,1,"Send Failed"),
      _lPadding(0,6,"Pad"),
      _lNoChannel(0,3,"No Free Channel"),
      _fDr(10,0,2),
      _fSw1(13,7,4),
      _fSw2(0,7,9),
      _fFSB(3,0,1),
      _fTemp(13,6,4),
      _fPower(14,0,2),
      _fNextCh(0,5,17),
      _fGpsLat(0,3,17),
      _fGpsLon(0,4,17),
      _fResult(3,3,16),
      _fGpsTime(0,5,16),
      _fDownSnr(12,2,5),
      _fPadding(4,6,3),
      _fDownRssi(0,2,11)
{}

LayoutSemtech::~LayoutSemtech() {}

void LayoutSemtech::display() {}

void LayoutSemtech::initial() 
{
    writeLabel(_lBlank);
}

void LayoutSemtech::display(bool success, mDot::snr_stats snr, mDot::rssi_stats rssi, int power, int fsb, int padding, int dr)
{
    char buf[17];
    size_t size;

    clear();
    startUpdate();

    writeLabel(_lDr);
    writeLabel(_lTemp);
    writeLabel(_lPower);
    writeLabel(_lPadding);
    if (_band == mDot::FB_915) {
    writeLabel(_lFSB);
    }
    if(success) {
        size = snprintf(buf, sizeof(buf), "DWN %3d dbm", rssi.last);
        writeField(_fDownRssi, buf, size);

        memset(buf, 0, sizeof(buf));
        size = snprintf(buf, sizeof(buf), " %2.1f", (float)snr.last / 10.0);
        writeField(_fDownSnr, buf, size);
    } else writeLabel(_lNoLink);

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%d",dr);
    writeField(_fDr, buf, size, true);

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%d",power);
    writeField(_fPower, buf, size, true);
    
    if (_band == mDot::FB_915) {
    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%d",fsb);
    writeField(_fFSB, buf, size, true);
    }
    
    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%d",padding);
    writeField(_fPadding, buf, size, true);

    endUpdate();
}

void LayoutSemtech::updateSw1(string Sw1, string Sw2, int dr, int power, int padding)
{
    size_t size;
    char buf[17];
    string temp;
    for(int i = Sw1.size(); i<4; i++) temp+=" ";
    temp+=Sw1;
    writeField(_fSw1, temp, true);
    startUpdate();
    if(Sw2=="Data Rate") {
        memset(buf, 0, sizeof(buf));
        size = snprintf(buf, sizeof(buf), "%d",dr);
        writeField(_fDr, buf, size, true);
    } else if(Sw2=="Power") {
        memset(buf, 0, sizeof(buf));
        size = snprintf(buf, sizeof(buf), "%d",power);
        writeField(_fPower, buf, size, true);
    } else if(Sw2=="Padding") {
        memset(buf, 0, sizeof(buf));
        size = snprintf(buf, sizeof(buf), "%d",padding);
        writeField(_fPadding, buf, size, true);
    }
    endUpdate();
}

void LayoutSemtech::updateSw2(string Sw2)
{
    writeField(_fSw2, Sw2, true);
}

void LayoutSemtech::sending()
{
    clear();
    writeLabel(_lSend);
}

void LayoutSemtech::sendResult(string str)
{
    clear();
    writeField(_fResult,str,true);
}


void LayoutSemtech::updateNextCh(int count_down)
{
    clear();
    size_t size;
    char buf[17];
    size = snprintf(buf, sizeof(buf), "Countdown:%d",count_down);
    writeField(_fNextCh, buf, size, true);
    writeLabel(_lNoChannel);
}

void LayoutSemtech::updateStats(bool GPS, GPSPARSER::longitude lon, GPSPARSER::latitude lat, struct tm time, float temp)
{
    char buf[17];
    size_t size;

    startUpdate();

    if(GPS) {
        size = snprintf(buf, sizeof(buf), "%d %d %d.%03d %c",
                        abs(lon.degrees),
                        lon.minutes,
                        (lon.seconds * 6) / 1000,
                        (lon.seconds * 6) % 1000,
                        (lon.degrees > 0) ? 'E' : 'W');
        writeField(_fGpsLon, buf, size, true);

        memset(buf, 0, sizeof(buf));
        size = snprintf(buf, sizeof(buf), "%d %d %d.%03d %c",
                        abs(lat.degrees),
                        lat.minutes,
                        (lat.seconds * 6) / 1000,
                        (lat.seconds * 6) % 1000,
                        (lat.degrees > 0) ? 'N' : 'S');
        writeField(_fGpsLat, buf, size, true);

        memset(buf, 0, sizeof(buf));
        size = snprintf(buf, sizeof(buf), "%02d:%02d %02d/%02d/%04d",
                        time.tm_hour,
                        time.tm_min,
                        time.tm_mon + 1,
                        time.tm_mday,
                        time.tm_year + 1900);
        writeField(_fGpsTime, buf, size, true);

    } else  writeLabel(_lNoGps);

    memset(buf, 0, sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%.1f",temp);
    writeField(_fTemp, buf, size, true);

    endUpdate();
}

