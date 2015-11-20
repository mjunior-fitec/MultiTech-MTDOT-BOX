#ifndef __LAYOUTCONFIRM_H__
#define __LAYOUTCONFIRM_H__

#include "Layout.h"

class LayoutConfirm : public Layout {
    public:
        LayoutConfirm(DOGS102* lcd);
        ~LayoutConfirm();

        void display();

    private:
        Label _lMsg1;
        Label _lMsg2;
        Label _lIns1;
        Label _lIns2;
        Label _lSw1;
        Label _lSw2;
};

#endif
