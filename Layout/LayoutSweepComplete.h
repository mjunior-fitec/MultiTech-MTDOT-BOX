#ifndef __LAYOUTSWEEPCOMPLETE_H__
#define __LAYOUTSWEEPCOMPLETE_H__

#include "Layout.h"

class LayoutSweepComplete : public Layout {
    public:
        LayoutSweepComplete(DOGS102* lcd);
        ~LayoutSweepComplete();

        void display();
        void updateId(uint32_t id);
        void updatePass(uint8_t pass);
        void updateFail(uint8_t fail);

    private:
        Label _lId;
        Label _lInfo;
        Label _lPass;
        Label _lFail;
        Label _lSw2;
        
        Field _fId;
        Field _fPass;
        Field _fFail;
};

#endif
