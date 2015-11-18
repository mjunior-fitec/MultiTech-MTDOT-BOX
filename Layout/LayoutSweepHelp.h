#ifndef __LAYOUTSWEEPHELP_H__
#define __LAYOUTSWEEPHELP_H__

#include "Layout.h"

class LayoutSweepHelp : public Layout {
    public:
        LayoutSweepHelp(DOGS102* lcd);
        ~LayoutSweepHelp();

        void display();

    private:
        Label _lMode;
        Label _lIns1;
        Label _lIns2;
        Label _lSw1;
        Label _lSw2;
};

#endif
