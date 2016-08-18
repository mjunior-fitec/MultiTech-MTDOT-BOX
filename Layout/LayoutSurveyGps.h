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

#ifndef __LAYOUTSURVEYGPS_H__
#define __LAYOUTSURVEYGPS_H__

#include "Layout.h"
#include "GPSPARSER.h"
#include "mDot.h"

class LayoutSurveyGps : public Layout{
public:
    LayoutSurveyGps(DOGS102* lcd, uint8_t band);
    ~LayoutSurveyGps();

    void display();
    void display(bool success, mDot::snr_stats snr, mDot::rssi_stats rssi,int power, int fsb, int padding, int dr);

    void sending();
    void initial();
    void updateSw2(string str);
    void sendResult(string str);
    void updateNextCh(int count_down);
    void updateSw1(string Sw1, string Sw2);
    void updateStats(bool GPS, GPSPARSER::longitude lon, GPSPARSER::latitude lat, struct tm time, float temp);

private:
    uint8_t _band;

    Label _lDR;
    Label _lFSB;
    Label _lTemp;
    Label _lPower;
    Label _lPadding;

    Field _fDr;
    Field _fSw1;
    Field _fSw2;
    Field _fFSB;
    Field _fTemp;
    Field _fPower;
    Field _fNoLink;
    Field _fGpsLat;
    Field _fGpsLon;
    Field _fGpsTime;
    Field _fDownSnr;
    Field _fPadding;
    Field _fDownRssi;
};
#endif
