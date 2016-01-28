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

#ifndef __LAYOUTSURVEYSUCCESS_H__
#define __LAYOUTSURVEYSUCCESS_H__

#include "Layout.h"
#include "LoRaHandler.h"
#include "GPSPARSER.h"

class LayoutSurveySuccess : public Layout {
    public:
        LayoutSurveySuccess(DOGS102* lcd);
        ~LayoutSurveySuccess();

        void display();
        void updateId(uint32_t id);
        void updateRate(std::string rate);
        void updatePower(uint32_t power);
        void updateStats(LoRaHandler::LoRaLink link);
        void updateGpsLatitude(GPSPARSER::latitude lat);
        void updateGpsLatitude(std::string msg);
        void updateGpsLongitude(GPSPARSER::longitude lon);
        void updateGpsTime(struct tm time);
        void updateInfo(std::string info);
        void updateSw1(std::string sw1);
        void updateSw2(std::string sw2);
        void updateCountdown(uint32_t seconds);
        void updatePassFail(uint8_t pass, uint8_t fail);

    private:
        Label _lId;
        Label _lDr;
        Label _lPwr;
        Label _lUp;
        Label _lDown;
        
        Field _fId;
        Field _fDr;
        Field _fPwr;
        Field _fUpMargin;
        Field _fDownRssi;
        Field _fDownSnr;
        Field _fGpsLat;
        Field _fGpsLon;
        Field _fGpsTime;
        Field _fInfo;
        Field _fSw1;
        Field _fSw2;
};

#endif
