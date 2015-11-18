#ifndef __LAYOUTDEMOHELP_H__
#define __LAYOUTDEMOHELP_H__

#include "Layout.h"

class LayoutDemoHelp : public Layout {
    public:
        LayoutDemoHelp(DOGS102* lcd);
        ~LayoutDemoHelp();

        void display();

    private:
        Label _lMode;
        Label _lDesc;
        Label _lIns1;
        Label _lIns2;
        Label _lSw1;
        Label _lSw2;
};

#endif
