#ifndef __LORAHANDLER_H__
#define __LORAHANDLER_H__

#include "mDot.h"

#define loraSignal (int32_t)0x02

class LoRaHandler {
    public:
        typedef enum {
            none = 0,
            busy,
            link_check_success,
            send_success,
            join_success,
            link_check_failure,
            send_failure,
            join_failure
        } LoRaStatus;

        typedef struct {
            bool status;
            mDot::link_check up;
            mDot::ping_response down;
        } LoRaLink;

        typedef enum {
            green = 0,
            red
        } LedColor;

        LoRaHandler(osThreadId main);
        ~LoRaHandler();

        bool linkCheck();
        bool send(std::vector<uint8_t> data);
        bool join();
        bool action(uint8_t cmd);
        LoRaStatus getStatus();
        LoRaLink getLinkCheckResults();
        uint32_t getJoinAttempts();
        void resetJoinAttempts();
        void blinker();
        void resetActivityLed();

        osThreadId _main;
        Thread _thread;
        LoRaStatus _status;
        LoRaLink _link;
        mDot* _dot;
        Mutex _mutex;
        uint32_t _join_attempts;
        DigitalInOut _activity_led;
        Ticker _tick;
};

#endif

