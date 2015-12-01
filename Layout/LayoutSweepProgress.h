#ifndef __LAYOUTSWEEPPROGRESS_H__
#define __LAYOUTSWEEPPROGRESS_H__

#include "Layout.h"

class LayoutSweepProgress : public Layout {
    public:
        LayoutSweepProgress(DOGS102* lcd);
        ~LayoutSweepProgress();

        void display();

        void updateProgress(uint8_t complete, uint8_t total);
        void updateCountdown(uint32_t seconds);

    private:
        Label _lMsg1;
        Label _lMsg2;
        Label _lMsg3;

        Field _fProgress;
        Field _fCountdownLabel;
        Field _fCountdown;
};

#endif
