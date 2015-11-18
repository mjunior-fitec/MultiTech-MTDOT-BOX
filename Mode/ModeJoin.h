#ifndef __MODEJOIN_H__
#define __MODEJOIN_H__

#include "Mode.h"
#include "LayoutJoin.h"
#include "mDot.h"
#include "LoRaHandler.h"

class ModeJoin : public Mode {
    public:
        ModeJoin(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, uint8_t band);
        ~ModeJoin();

        bool start();

    private:
        LayoutJoin _lj;
        mDot* _dot;
        LoRaHandler* _lora;
        uint8_t _band;
        uint8_t _sub_band;
        uint8_t _data_rate;
        uint8_t _power;
        uint32_t _next_tx;
        bool _joined;
        ButtonHandler::ButtonEvent _be;
        LoRaHandler::LoRaStatus _ls;
};

#endif
