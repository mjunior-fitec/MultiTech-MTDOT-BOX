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

#ifndef __LAYOUTSURVEYFAILURE_H__
#define __LAYOUTSURVEYFAILURE_H__

#include "Layout.h"
#include "GPSPARSER.h"

class LayoutSurveyFailure : public Layout {
    public:
        LayoutSurveyFailure(DOGS102* lcd);
        ~LayoutSurveyFailure();

        void display();
        void updateId(uint32_t id);
        void updateRate(std::string rate);
        void updatePower(uint32_t power);
        void updateGpsLatitude(GPSPARSER::latitude lat);
        void updateGpsLatitude(std::string msg);
        void updateGpsLongitude(GPSPARSER::longitude lon);
        void updateGpsTime(struct tm time);
        void updateInfo(std::string info);
        void updateSw1(std::string sw1);
        void updateSw2(std::string sw2);
        void updatePassFail(uint8_t pass, uint8_t fail);

    private:
        Label _lTitle;
        Label _lId;
        Label _lDr;
        Label _lPwr;

        Field _fId;
        Field _fDr;
        Field _fPwr;
        Field _fGpsLat;
        Field _fGpsLon;
        Field _fGpsTime;
        Field _fInfo;
        Field _fSw1;
        Field _fSw2;
};

#endif
