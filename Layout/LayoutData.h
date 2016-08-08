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

#ifndef __LAYOUTDATA_H__
#define __LAYOUTDATA_H__

#include "Layout.h"


class LayoutData : public Layout
{
public:
    LayoutData(DOGS102* lcd);
    ~LayoutData();

    struct singleLine {
        string id,
               status,
               lock,
               lat,
               lng,
               alt,
               time,
               gateways,
               margin,
               rssiD,
               snrD,
               dataRate,
               power;
    } ;

    void display();
    void noData();
    void noGps();
    void errorData();

    void updateSw1(string str);
    void updateSw2(string str);
    bool updateAll(singleLine& line);

private:
    Label _lDr;
    Label _lPwr;
    Label _lUp;
    Label _lGw;
    Label _lDown;
    Label _lDbm;
    Label _lNoData;
    Label _lSurveyFailed;
    Label _lErrorData1;
    Label _lErrorData2;
    Label _lNoGps;
    Label _lAlt;

    Field _fId;
    Field _fDr;
    Field _fPwr;
    Field _fUpMargin;
    Field _fRssiDown;
    Field _fSnrDown;
    Field _fGpsLat;
    Field _fGpsLong;
    Field _fGpsTime;
    Field _fGw;
    Field _fAlt;
    Field _fSw1;
    Field _fSw2;

};

#endif




