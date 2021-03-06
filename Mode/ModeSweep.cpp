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

#include "ModeSweep.h"
#include "MTSLog.h"

ModeSweep::ModeSweep(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors)
  : Mode(lcd, buttons, dot, lora, gps, sensors),
    _help(lcd),
    _file(lcd),
    _confirm(lcd),
    _progress(lcd),
    _success(lcd),
    _failure(lcd),
    _complete(lcd)
{}

ModeSweep::~ModeSweep() {}

bool ModeSweep::start() {
    bool data_file = false;
    bool send_link_check = false;
    bool send_data = false;
    bool no_channel_link_check = false;
    bool no_channel_data = false;

    // clear any stale signals
    osSignalClear(_main_id, buttonSignal | loraSignal);

    _initial_data_rate = _dot->getTxDataRate();
    _initial_power = _dot->getTxPower();

    // see if we're supposed to send the data packet after success
    // that item is stored in the mDot::StartUpMode config field
    _send_data = _dot->getStartUpMode();

    // pull the minimum and maximum payload size out of config
    // min payload size is wake interval
    // max payload size is wake delay
    _min_payload = _dot->getWakeInterval();
    _max_payload = _dot->getWakeDelay();

    // pull the minimum and maximum power out of config
    // min power is wake timeout
    // max power is wake mode
    _min_power = _dot->getWakeTimeout();
    _max_power = _dot->getWakeMode();
    
    // compute the total number of surveys we will do
    _points = generatePoints();
    _survey_total = _points.size();
    _survey_current = 1;
    _survey_success = 0;
    _survey_failure = 0;

    // see if survey data file exists
    std::vector<mDot::mdot_file> files = _dot->listUserFiles();
    for (std::vector<mDot::mdot_file>::iterator it = files.begin(); it != files.end(); it++) {
        if (strcmp(it->name, file_name) == 0) {
            logInfo("found survey data file");
            data_file = true;
            break;
        }
    }
    if (data_file) {
        _state = check_file;
        _file.display();
    } else {
        _state = show_help;
        _index = 1;
        displayHelp();
    }

    _display_timer.reset();

    while (true) {
        osEvent e = Thread::signal_wait(0, 250);
        if (e.status == osEventSignal) {
            if (e.value.signals & buttonSignal) {
                _be = _buttons->getButtonEvent();

                switch (_be) {
                    case ButtonHandler::sw1_press:
                        switch (_state) {
                            case check_file:
                                _state = show_help;
                                _index = getIndex(sweep) + 1;
                                displayHelp();
                                break;
                            case confirm:
                                _state = check_file;
                                _file.display();
                                break;
                            case success:
                                _state = complete;
                                _display_timer.stop();
                                _display_timer.reset();
                                logInfo("sweep finished");
                                _complete.display();
                                _complete.updateId(_index++);
                                _complete.updatePass(_survey_success);
                                _complete.updateFail(_survey_failure);
                                _survey_success = 0;
                                _survey_failure = 0;
                                break;
                            case failure:
                                _state = complete;
                                _display_timer.stop();
                                _display_timer.reset();
                                logInfo("sweep finished");
                                _complete.display();
                                _complete.updateId(_index++);
                                _complete.updatePass(_survey_success);
                                _complete.updateFail(_survey_failure);
                                _survey_success = 0;
                                _survey_failure = 0;
                                break;
                        }
                        break;

                    case ButtonHandler::sw2_press:
                        switch (_state) {
                            case check_file:
                                _state = confirm;
                                _confirm.display();
                                break;
                            case confirm:
                                _state = show_help;
                                logInfo("deleting survey data file");
                                _dot->deleteUserFile(file_name);
                                _index = 1;
                                displayHelp();
                                break;
                            case show_help:
                                _state = in_progress;
                                _progress.display();
                                _progress.updateProgress(_survey_current, _survey_total);
                                if (_dot->getNextTxMs() > 0)
                                    no_channel_link_check = true;
                                else
                                    send_link_check = true;
                                break;
                            case complete:
                                _state = in_progress;
                                _survey_current = 1;
                                _progress.display();
                                _progress.updateProgress(_survey_current, _survey_total);
                                if (_dot->getNextTxMs() > 0)
                                    no_channel_link_check = true;
                                else
                                    send_link_check = true;
                                break;
                        }
                        break;
                    case ButtonHandler::sw1_hold:
                        _dot->setTxDataRate(_initial_data_rate);
                        _dot->setTxPower(_initial_power);
                        return true;
                }
            }
            if (e.value.signals & loraSignal) {
                _ls = _lora->getStatus();
                switch (_ls) {
                    case LoRaHandler::link_check_success:
                        switch (_state) {
                            case in_progress:
                                _survey_success++;
                                _link_check_result = _lora->getLinkCheckResults();
                                displaySuccess();
                                logInfo("link check successful\tMargin %ld\tRSSI %d dBm\tSNR %2.3f", _link_check_result.up.dBm, _link_check_result.down.rssi, (float)_link_check_result.down.snr / 10.0);
                                updateData(_data, sweep, true);
                                appendDataFile(_data);
                                if (_send_data) {
                                    _state = data;
                                    if (_dot->getNextTxMs() > 0)
                                        no_channel_data = true;
                                    else
                                        send_data = true;
                                } else {
                                    _state = success;
                                    _success.updateSw1("  Cancel");
                                    _display_timer.start();
                                }
                                break;
                        }
                        break;

                    case LoRaHandler::link_check_failure:
                        switch (_state) {
                            case in_progress:
                                _survey_failure++;
                                _state = failure;
                                _failure.display();
                                _failure.updateId(_index);
                                _failure.updateRate(_dot->DataRateStr(_data_rate).substr(2));
                                _failure.updatePower(_power);
                                if (_gps_available && _gps->getLockStatus()) {
                                    GPSPARSER::latitude lat = _gps->getLatitude();
                                    GPSPARSER::longitude lon = _gps->getLongitude();
                                    _failure.updateGpsLatitude(lat);
                                    _failure.updateGpsLongitude(lon);
                                } else {
                                    _failure.updateGpsLatitude("No GPS Lock");
                                }
                                _failure.updatePassFail(_survey_success, _survey_failure);
                                _failure.updateSw1("  Cancel");
                                updateData(_data, sweep, false);
                                appendDataFile(_data);
                                logInfo("link check failed");
                                _display_timer.start();
                                break;
                        }
                        break;

                    case LoRaHandler::send_success:
                        switch (_state) {
                            case data:
                                _state = success;
                                _success.updateInfo("                 ");
                                _success.updateSw1("  Cancel");
                                logInfo("data send success");
                                // turn acks and receive windows back on
                                _dot->setAck(1);
                                _dot->setTxWait(true);
                                _display_timer.start();
                                break;
                        }
                        break;

                    case LoRaHandler::send_failure:
                        switch (_state) {
                            case data:
                                _state = success;
                                _success.updateInfo("                 ");
                                _success.updateSw1("  Cancel");
                                logInfo("data send failed");
                                // turn acks and receive windows back on
                                _dot->setAck(1);
                                _dot->setTxWait(true);
                                _display_timer.start();
                                break;
                        }
                        break;
                }
            }
        }

        // wait 5s in EU mode to compensate for potential "no free channel" situations on server
        if ((_band != mDot::FB_EU868 && _display_timer.read_ms() > 2000) || _display_timer.read_ms() > 5000) {
            _display_timer.stop();
            _display_timer.reset();
            if (_survey_current == _survey_total) {
                logInfo("sweep finished");
                _state = complete;
                _complete.display();
                _complete.updateId(_index++);
                _complete.updatePass(_survey_success);
                _complete.updateFail(_survey_failure);
                _survey_success = 0;
                _survey_failure = 0;
            } else {
                logInfo("starting next link check");
                _state = in_progress;
                _survey_current++;
                _progress.display();
                _progress.updateProgress(_survey_current, _survey_total);
                no_channel_link_check = true;
            }
        }

        if (no_channel_link_check) {
            uint32_t t = _dot->getNextTxMs();
            if (t > 0) {
                logInfo("next tx %lu ms", t);
                _progress.updateCountdown(t / 1000);
            } else {
                _progress.display();
                _progress.updateProgress(_survey_current, _survey_total);
                no_channel_link_check = false;
                send_link_check = true;
            }
        }
        if (no_channel_data) {
            uint32_t t = _dot->getNextTxMs();
            if (t > 0) {
                logInfo("next tx %lu ms", t);
                _success.updateCountdown(t / 1000);
            } else {
                displaySuccess();
                no_channel_data = false;
                send_data = true;
            }
        }
        if (send_link_check) {
            point p = _points[_survey_current - 1];
            _data_rate = p.first;
            _power = p.second;
            logInfo("sending link check %s %d", _dot->DataRateStr(_data_rate).c_str(), _power);
            send_link_check = false;
            _dot->setTxDataRate(_data_rate);
            _dot->setTxPower(_power);
            _lora->linkCheck();
        }
        if (send_data) {
            std::vector<uint8_t> s_data = formatSurveyData(_data);
            logInfo("sending data %s %d", _dot->DataRateStr(_data_rate).c_str(), _power);
            send_data = false;
            _success.updateInfo("Data Sending...");
            _dot->setTxDataRate(_data_rate);
            _dot->setTxPower(_power);
            // we don't care if the server actually gets this packet or not
            // we won't retry anyway
            _dot->setAck(0);
            _dot->setTxWait(false);
            _lora->send(s_data);
            osDelay(500);
        }
    }
}

void ModeSweep::displayHelp() {
    _help.display();
    _help.updateMode("Survey Sweep");
    _help.updateSw2("Sweep");
}

void ModeSweep::displaySuccess() {
    _success.display();
    _success.updateId(_index);
    _success.updateRate(_dot->DataRateStr(_data_rate).substr(2));
    _success.updatePower(_power);
    _success.updateStats(_link_check_result);
    if (_gps_available && _gps->getLockStatus()) {
        GPSPARSER::latitude lat = _gps->getLatitude();
        GPSPARSER::longitude lon = _gps->getLongitude();
        _success.updateGpsLatitude(lat);
        _success.updateGpsLongitude(lon);
    } else {
        _success.updateGpsLatitude("No GPS Lock");
    }
    _success.updatePassFail(_survey_success, _survey_failure);
}

std::vector<point> ModeSweep::generatePoints() {
    std::vector<point> p;
    uint8_t min_rate;
    uint8_t max_rate;

    max_rate = payloadToRate(_min_payload);
    min_rate = payloadToRate(_max_payload);

    for (int rate = min_rate; rate >= max_rate; rate--) {
        if (_max_power - _min_power < 4) {
            for (uint32_t power = _min_power; power <= _max_power; power++)
                p.push_back(std::make_pair(rate, power));
        } else {
            p.push_back(std::make_pair(rate, _min_power));
            p.push_back(std::make_pair(rate, (uint32_t)ceil( (float(_max_power) - float(_min_power)) * 0.33 + float(_min_power))));
            p.push_back(std::make_pair(rate, (uint32_t)ceil( (float(_max_power) - float(_min_power)) * 0.66 + float(_min_power))));
            p.push_back(std::make_pair(rate, _max_power));
        }
    }

    return p;
}

uint8_t ModeSweep::payloadToRate(uint8_t payload) {
    if (_band == mDot::FB_EU868) {
        if (payload <= mDot::MaxLengths_868[mDot::DR0])
            return mDot::DR0;
        else if (payload <= mDot::MaxLengths_868[mDot::DR3])
            return mDot::DR3;
        else
            return mDot::DR6;
    } else {
        if (payload <= mDot::MaxLengths_915[mDot::DR0])
            return mDot::DR0;
        else if (payload <= mDot::MaxLengths_915[mDot::DR1])
            return mDot::DR1;
        else if (payload <= mDot::MaxLengths_915[mDot::DR2])
            return mDot::DR2;
        else
            return mDot::DR4;
    }
}

