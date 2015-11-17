#ifndef __LAYOUTCONFIG_H__
#define __LAYOUTCONFIG_H__

#include "Layout.h"

class LayoutConfig : public Layout {
    public:
        LayoutConfig(DOGS102* lcd);
        ~LayoutConfig();

        void display();

    private:
        Label _lMode;
        Label _lHelp1;
        Label _lHelp2;
        Label _lHelp3;
};

#endif
