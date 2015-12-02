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
