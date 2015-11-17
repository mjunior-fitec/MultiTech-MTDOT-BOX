#ifndef __LAYOUTJOIN_H__
#define __LAYOUTJOIN_H__

#include "Layout.h"
#include "mDot.h"

class LayoutJoin : public Layout {
    public:
        LayoutJoin(DOGS102* lcd, uint8_t band);
        ~LayoutJoin();

        void display();

        void updateId(std::string id);
        void updateKey(std::string key);
        void updateFsb(uint8_t band);
        void updateRate(std::string rate);
        void updatePower(uint32_t power);
        void updateStatus(std::string status);
        void updateCountdown(uint32_t seconds);
        void removeCountdown();
        void displayCancel(bool display = true);

    private:
        Label _lId;
        Label _lKey;
        Label _lFsb;
        Label _lRate;
        Label _lPower;

        Field _fStatus;
        Field _fId;
        Field _fKey;
        Field _fFsb;
        Field _fRate;
        Field _fPower;
        Field _fCountdown;
        Field _fCountdownLabel;
        Field _fCancel;
        uint8_t _band;
};

#endif
