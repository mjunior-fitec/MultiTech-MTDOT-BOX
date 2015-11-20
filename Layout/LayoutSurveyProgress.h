#ifndef __LAYOUTSURVEYPROGRESS_H__
#define __LAYOUTSURVEYPROGRESS_H__

#include "Layout.h"

class LayoutSurveyProgress : public Layout {
    public:
        LayoutSurveyProgress(DOGS102* lcd);
        ~LayoutSurveyProgress();

        void display();

        void updateCountdown(uint32_t seconds);

    private:
        Label _lMsg1;
        Label _lMsg2;
        Label _lMsg3;

        Field _fCountdownLabel;
        Field _fCountdown;
};

#endif
