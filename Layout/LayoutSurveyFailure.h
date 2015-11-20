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
        void updateInfo(std::string msg);

    private:
        Label _lTitle;
        Label _lId;
        Label _lDr;
        Label _lPwr;
        Label _lSw1;
        Label _lSw2;

        Field _fId;
        Field _fDr;
        Field _fPwr;
        Field _fMsg1;
        Field _fMsg2;
        Field _fInfo;
};

#endif
