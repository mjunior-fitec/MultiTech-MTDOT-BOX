#ifndef __LAYOUTSTARTUP_H__
#define __LAYOUTSTARTUP_H__

#include "Layout.h"

class LayoutStartup : public Layout {
    public:
        LayoutStartup(DOGS102* lcd);
        ~LayoutStartup();

        void display();

    private:
        Label _lName;
        Label _lInfo;
        Label _lVersion;

        Field _fVersion;

        Image _iLogo;
};

#endif
