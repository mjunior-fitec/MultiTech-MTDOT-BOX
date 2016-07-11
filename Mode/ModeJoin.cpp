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

#include "ModeJoin.h"
#include "MTSLog.h"
#include "MTSText.h"

ModeJoin::ModeJoin(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors)
  : Mode(lcd, buttons, dot, lora, gps, sensors),
    _join(lcd, _band),
    _joined(false)
{}

ModeJoin::~ModeJoin() {}

bool ModeJoin::start() {
    bool joining = false;

    // clear any stale signals
    osSignalClear(_main_id, buttonSignal | loraSignal);

    if (_dot->getJoinMode() == mDot::MANUAL) {
        // already joined
        return true;
    }

    _initial_data_rate = _dot->getTxDataRate();
    _initial_power = _dot->getTxPower();

    _data_rate = (_band == mDot::FB_915) ? mDot::SF_10 : mDot::SF_12;
    _power = 20;
    _joined = false;

    display();

    _dot->setTxDataRate(_data_rate);
    _dot->setTxPower(_power);
    _lora->resetJoinAttempts();

    while (! _joined) {
        _next_tx = _dot->getNextTxMs();
        if (! joining && _next_tx > 0) {
            logInfo("next tx %lu ms", _next_tx);
            _join.updateStatus("Waiting...");
            _join.updateCountdown(_next_tx / 1000);
        } else if (! joining) {
            logInfo("attempting to join");
            joining = true;
            display();
            _lora->join();
        }

        osEvent e = Thread::signal_wait(0, 250);
        if (e.status == osEventSignal) {
            if (e.value.signals & buttonSignal) {
                _be = _buttons->getButtonEvent();
                switch (_be) {
                    case ButtonHandler::sw1_press:
                        _dot->setTxDataRate(_initial_data_rate);
                        _dot->setTxPower(_initial_power);
                        return false;
                    case ButtonHandler::sw2_press:
                        break;
                    case ButtonHandler::sw1_hold:
                        _dot->setTxDataRate(_initial_data_rate);
                        _dot->setTxPower(_initial_power);
                        return false;
                }
            }
            if (e.value.signals & loraSignal) {
                _ls = _lora->getStatus();
                switch (_ls) {
                    case LoRaHandler::join_success:
                        _join.updateStatus("Join Success!");
                        _join.displayCancel(false);
                        logInfo("joined");
                        _joined = true;
                        osDelay(2000);
                        _dot->setTxDataRate(_initial_data_rate);
                        _dot->setTxPower(_initial_power);
                        return true;

                    case LoRaHandler::join_failure:
                        logInfo("failed to join");
                        joining = false;
                        break;
                }
            }
        }
    }

    _dot->setTxDataRate(_initial_data_rate);
    _dot->setTxPower(_initial_power);
    return false;
}

void ModeJoin::display() {
    _join.display();
    _join.updateStatus("Joining...");
    if (_dot->getJoinMode() == mDot::MANUAL) {
        _join.updateId(mts::Text::bin2hexString(_dot->getNetworkId()));
        _join.updateKey(mts::Text::bin2hexString(_dot->getNetworkKey()));
    } else {
        _join.updateId(_dot->getNetworkName());
        _join.updateKey(_dot->getNetworkPassphrase());
    }
    if (_band == mDot::FB_915) {
        _sub_band = _dot->getFrequencySubBand();
        _join.updateFsb(_sub_band);
    }
    // mDot::DataRateStr returns format SF_XX - we only want to display the XX part
    _join.updateRate(_dot->DataRateStr(_data_rate).substr(3));
    _join.updatePower(_power);
    _join.updateAttempt(_lora->getJoinAttempts());
}

