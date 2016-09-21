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

#ifndef __MODESEMTECH_H__
#define __MODESEMTECH_H__

#include "Mode.h"
#include "ModeJoin.h"
#include "LayoutHelp.h"
#include "LayoutSemtech.h"
#include "LayoutSemtechJoin.h"

class ModeSemtech : public Mode
{
public:
    ModeSemtech(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors);
    ~ModeSemtech();

    bool start();

private:
    enum {DATA_RATE, FSB, PADDING, POWER, INTERVAL};
    enum {BAND_CHANGE, SENDING, PARAMETERS};

    LayoutHelp _help;
    LayoutSemtech _sem;
    LayoutSemtechJoin _sem_join;

    ModeJoin* _join;

    Timer _send_timer, _button_timer;

    float _temp_C;
    std::vector<uint8_t> _send_data;
    uint8_t _parameter, _padding, _interval,_max_padding, _band;
    bool _drAll, _link_check, _GPS;
    string _Sw1, _Sw2;

    struct tm _time;
    mDot::snr_stats _snr;
    mDot::rssi_stats _rssi;
    GPSPARSER::latitude  _latitude;
    GPSPARSER::longitude _longitude;

    void init();
    void send();
    void setBand();
    void sendData();
    void formatData();
    void changeBand();
    void drIncrement();
    void updateScreen();
    void editParameter();
    void changeDataRate();
    void changeParameter();
    string intToString(int num);

};

#endif

