#ifndef __LAYOUTSURVEYFAILURE_H__
#define __LAYOUTSURVEYFAILURE_H__

#include "Layout.h"

class LayoutSurveyFailure : public Layout {
    public:
        LayoutSurveyFailure(DOGS102* lcd);
        ~LayoutSurveyFailure();

        void display();
        void updateId(uint32_t id);
        void updateRate(std::string rate);
        void updatePower(uint32_t power);
        void updateInfo1(std::string msg);
        void updateInfo2(std::string msg);
        void updatePassFail(uint8_t pass, uint8_t fail);
        void updateSw1(std::string sw1);
        void updateSw2(std::string sw2);

    private:
        Label _lTitle;
        Label _lId;
        Label _lDr;
        Label _lPwr;

        Field _fId;
        Field _fDr;
        Field _fPwr;
        Field _fMsg1;
        Field _fMsg2;
        Field _fInfo1;
        Field _fInfo2;
        Field _fSw1;
        Field _fSw2;
};

#endif
