/* Copyright (c) <2016> <MultiTech Systems>, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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

