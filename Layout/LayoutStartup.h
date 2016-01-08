#ifndef __LAYOUTSTARTUP_H__
#define __LAYOUTSTARTUP_H__

#include "Layout.h"
#include "mDot.h"

class LayoutStartup : public Layout {
    public:
        LayoutStartup(DOGS102* lcd, mDot* dot);
        ~LayoutStartup();

        void display();
        void updateGPS(bool gps);

    private:
        mDot* _dot;

        Label _lName;
        Label _lInfo;
        Label _lVersion;

        Field _fName;
        Field _fVersion;
        Field _fGps;
        Field _fId;

        Image _iLogo;
};

#endif
