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
        void updateStats(LoRaHandler::LoRaPing ping);
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
        Field _fUpRssi;
        Field _fUpSnr;
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
