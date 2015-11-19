#ifndef __MODECONFIG_H__
#define __MODECONFIG_H__

#include "Mode.h"
#include "LayoutConfig.h"
#include "mDot.h"

class ModeConfig : public Mode {
    public:
        ModeConfig(DOGS102* lcd, ButtonHandler* buttons, mDot* dot);
        ~ModeConfig();

        bool start();

    private:
        LayoutConfig _lc;
        mDot* _dot;
        ButtonHandler::ButtonEvent _be;
};

#endif
