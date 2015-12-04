#ifndef __MODEDEMO_H__
#define __MODEDEMO_H__

#include "Mode.h"
#include "LayoutHelp.h"
#include "LayoutDemoSampling.h"

class ModeDemo : public Mode {
    public:
        ModeDemo(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps);
        ~ModeDemo();

        bool start();

    private:
        void displayHelp();

        typedef enum {
            show_help = 0,
            sampling,
            success,
            failure
        } state;

        typedef enum {
            trigger = 0,
            interval
        } mode;

        LayoutHelp _help;
        LayoutDemoSampling _sam;
        SensorItem _data;
        mode _mode;
        Timer _send_timer;
        uint8_t _interval;
        static const uint32_t _intervals[];
};

#endif
