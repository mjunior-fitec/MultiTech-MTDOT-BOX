#ifndef __MODEJOIN_H__
#define __MODEJOIN_H__

#include "Mode.h"
#include "LayoutJoin.h"

class ModeJoin : public Mode {
    public:
        ModeJoin(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps);
        ~ModeJoin();

        bool start();

    private:
        void display();

        LayoutJoin _join;
        bool _joined;
};

#endif
