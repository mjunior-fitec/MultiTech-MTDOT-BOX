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

#include "LayoutData.h"


LayoutData::LayoutData(DOGS102* lcd)
    : Layout(lcd),
      _lDr(8, 0, "DR"),
      _lPwr(13, 0, "P"),
      _lUp(0, 1, "UP Mgn"),
      _lGw(10, 1, "Gw"),
      _lDown(0, 2, "DWN -"),
      _lNoData(1,3,"No Survey Data"),
      _lErrorData1(1,3,"Error opening,"),
      _lErrorData2(0,4,"survey data file."),
      _lSurveyFailed(0,1,"Survey Failed"),
      _lNoGps(0,3,"No GPS Data"),
      _lDbm(9,2,"dbm"),
      _lAlt(0,6,"Alt"),
      _fId(0, 0, 5),
      _fDr(10, 0, 2),
      _fPwr(14, 0, 2),
      _fUpMargin(7, 1, 2),
      _fGw(13, 1, 2),
      _fRssiDown(5, 2, 3),
      _fSnrDown(13, 2, 4),
      _fGpsLat(0, 4, 17),
      _fGpsLong(0, 3, 17),
      _fGpsTime(0, 5, 17),
      _fAlt(4,6,13),
      _fSw1(12, 7, 4),
      _fSw2(0, 7, 4)
{}

LayoutData::~LayoutData() {}

void LayoutData::display()
{
    clear();
    startUpdate();
    writeLabel(_lDr);
    writeLabel(_lPwr);
    endUpdate();
}

void LayoutData::noData()
{
    clear();
    writeLabel(_lNoData);
}

void LayoutData::errorData()
{
    clear();
    writeLabel(_lErrorData1);
    writeLabel(_lErrorData2);
}

void LayoutData::updateSw1(string str)
{
    writeField(_fSw1,str,true);
}

void LayoutData::updateSw2(string str)
{
    writeField(_fSw2,str,true);
}

bool LayoutData::updateAll(singleLine& line)
{
    clear();
    startUpdate();
    //this data should always exist
    writeLabel(_lDr);
    writeLabel(_lPwr);
    writeField(_fId,line.id,true);
    writeField(_fDr,line.dataRate,true);
    writeField(_fPwr,line.power,true);
    //check if survey pass/fail
    if(line.gateways!="") {
        writeLabel(_lUp);
        writeLabel(_lDown);
        writeLabel(_lGw);
        writeLabel(_lDbm);
        writeField(_fGw,line.gateways,true);
        writeField(_fUpMargin,line.margin,true);
        writeField(_fRssiDown,line.rssiD,true);
        writeField(_fSnrDown,line.snrD,true);
    } else writeLabel(_lSurveyFailed);
    //check if gps data exists
    if(line.lat!="") {
        writeLabel(_lAlt);
        writeField(_fGpsLat,line.lat,true);
        writeField(_fGpsLong,line.lng,true);
        writeField(_fGpsTime,line.time,true);
        writeField(_fAlt,line.alt + " m",true);
    } else writeLabel(_lNoGps);

    endUpdate();
}




