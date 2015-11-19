#ifndef __LAYOUTHELP_H__
#define __LAYOUTHELP_H__

#include "Layout.h"

class LayoutHelp : public Layout {
    public:
        LayoutHelp(DOGS102* lcd);
        ~LayoutHelp();

        void display();
        void updateMode(std::string mode);
        void updateDescription(std::string description);
        void updateMsg(std::string msg);
        void removeMsg();
        void updateSw1(std::string s);
        void updateSw2(std::string s);

    private:
        Label _lIns1;
        Label _lIns2;

        Field _fMode;
        Field _fDesc;
        Field _fMsg;
        Field _fSw1;
        Field _fSw2;

};

#endif
