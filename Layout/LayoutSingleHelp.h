#ifndef __LAYOUTSINGLEHELP_H__
#define __LAYOUTSINGLEHELP_H__

#include "Layout.h"

class LayoutSingleHelp : public Layout {
    public:
        LayoutSingleHelp(DOGS102* lcd);
        ~LayoutSingleHelp();

        void display();

    private:
        Label _lMode;
        Label _lIns1;
        Label _lIns2;
        Label _lSw1;
        Label _lSw2;
};

#endif
