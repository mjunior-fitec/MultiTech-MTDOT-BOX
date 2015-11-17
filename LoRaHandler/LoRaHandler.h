#ifndef __LORAHANDLER_H__
#define __LORAHANDLER_H__

#include "mDot.h"

#define loraSignal (int32_t)0x02

class LoRaHandler {
    public:
        typedef enum {
            none = 0,
            busy,
            ping_success,
            send_success,
            join_success,
            ping_failure,
            send_failure,
            join_failure
        } LoRaStatus;

        typedef struct {
            bool status;
            mDot::ping_response up;
            mDot::ping_response down;
        } LoRaPing;

        LoRaHandler(osThreadId main);
        ~LoRaHandler();

        bool setDataRate(uint8_t rate);
        bool setPower(uint32_t power);
        bool ping();
        bool send(std::vector<uint8_t> data);
        bool join();
        bool action(uint8_t cmd);
        LoRaStatus getStatus();
        LoRaPing getPingResults();
        uint32_t getNextTx();


        osThreadId _main;
        Thread _thread;
        LoRaStatus _status;
        LoRaPing _ping;
        mDot* _dot;
        Mutex _mutex;
};

#endif

