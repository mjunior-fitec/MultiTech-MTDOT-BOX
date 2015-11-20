#ifndef __LAYOUTFILE_H__
#define __LAYOUTFILE_H__

#include "Layout.h"

class LayoutFile : public Layout {
    public:
        LayoutFile(DOGS102* lcd);
        ~LayoutFile();

        void display();

    private:
        Label _lMsg1;
        Label _lMsg2;
        Label _lMsg3;
        Label _lIns1;
        Label _lIns2;
        Label _lSw1;
        Label _lSw2;
};

#endif
