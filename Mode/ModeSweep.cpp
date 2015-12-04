#include "ModeSweep.h"
#include "MTSLog.h"

ModeSweep::ModeSweep(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps)
  : Mode(lcd, buttons, dot, lora, gps),
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
    bool send_ping = false;
    bool send_data = false;
    bool no_channel_ping = false;
    bool no_channel_data = false;

    // clear any stale signals
    osSignalClear(_main_id, buttonSignal | loraSignal);

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

    logInfo("%u points", _survey_total);
    for (std::vector<point>::iterator it = _points.begin(); it != _points.end(); it++)
        logInfo("%s,%lu", _dot->DataRateStr(it->first).substr(3).c_str(), it->second);

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
                            case show_help:
                                break;
                            case in_progress:
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
                            case data:
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
                            case complete:
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
                                    no_channel_ping = true;
                                else
                                    send_ping = true;
                                break;
                            case in_progress:
                                break;
                            case success:
                                break;
                            case data:
                                break;
                            case failure:
                                break;
                            case complete:
                                _state = in_progress;
                                _survey_current = 1;
                                _progress.display();
                                _progress.updateProgress(_survey_current, _survey_total);
                                if (_dot->getNextTxMs() > 0)
                                    no_channel_ping = true;
                                else
                                    send_ping = true;
                                break;
                        }
                        break;
                    case ButtonHandler::sw1_hold:
                        return true;
                }
            }
            if (e.value.signals & loraSignal) {
                _ls = _lora->getStatus();
                switch (_ls) {
                    case LoRaHandler::ping_success:
                        switch (_state) {
                            case check_file:
                                break;
                            case confirm:
                                break;
                            case show_help:
                                break;
                            case in_progress:
                                _survey_success++;
                                _ping_result = _lora->getPingResults();
                                displaySuccess();
                                logInfo("ping successful");
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
                            case success:
                                break;
                            case data:
                                break;
                            case failure:
                                break;
                            case complete:
                                break;
                        }
                        break;

                    case LoRaHandler::ping_failure:
                        switch (_state) {
                            case check_file:
                                break;
                            case confirm:
                                break;
                            case show_help:
                                break;
                            case in_progress:
                                _survey_failure++;
                                _state = failure;
                                _failure.display();
                                _failure.updateId(_index);
                                // mDot::DataRateStr returns format SF_XX - we only want to display the XX part
                                _failure.updateRate(_dot->DataRateStr(_data_rate).substr(3));
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
                                logInfo("ping failed");
                                _display_timer.start();
                                break;
                            case success:
                                break;
                            case data:
                                break;
                            case failure:
                                break;
                            case complete:
                                break;
                        }
                        break;

                    case LoRaHandler::send_success:
                        switch (_state) {
                            case check_file:
                                break;
                            case confirm:
                                break;
                            case show_help:
                                break;
                            case in_progress:
                                break;
                            case success:
                                break;
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
                            case failure:
                                break;
                            case complete:
                                break;
                        }
                        break;

                    case LoRaHandler::send_failure:
                        switch (_state) {
                            case check_file:
                                break;
                            case confirm:
                                break;
                            case show_help:
                                break;
                            case in_progress:
                                break;
                            case success:
                                break;
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
                            case failure:
                                break;
                            case complete:
                                break;
                        }
                        break;
                }
            }
        }

        if (_display_timer.read_ms() > 2000) {
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
                logInfo("starting next ping");
                _state = in_progress;
                _survey_current++;
                _progress.display();
                _progress.updateProgress(_survey_current, _survey_total);
                no_channel_ping = true;
            }
        }

        if (no_channel_ping) {
            uint32_t t = _dot->getNextTxMs();
            if (t > 0) {
                logInfo("next tx %lu ms", t);
                _progress.updateCountdown(t / 1000);
            } else {
                _progress.display();
                _progress.updateProgress(_survey_current, _survey_total);
                no_channel_ping = false;
                send_ping = true;
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
        if (send_ping) {
            point p = _points[_survey_current - 1];
            _data_rate = p.first;
            _power = p.second;
            logInfo("sending ping %s %d", _dot->DataRateStr(_data_rate).c_str(), _power);
            send_ping = false;
            _dot->setTxDataRate(_data_rate);
            _dot->setTxPower(_power);
            _lora->ping();
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
    _help.updateSw1("  Cancel");
    _help.updateSw2("Sweep");
}

void ModeSweep::displaySuccess() {
    _success.display();
    _success.updateId(_index);
    // mDot::DataRateStr returns format SF_XX - we only want to display the XX part
    _success.updateRate(_dot->DataRateStr(_data_rate).substr(3));
    _success.updatePower(_power);
    _success.updateStats(_ping_result);
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
    if (_band == mDot::FB_915) {
        if (payload <= mDot::MaxLengths_915[mDot::SF_10])
            return mDot::SF_10;
        else if (payload <= mDot::MaxLengths_915[mDot::SF_9])
            return mDot::SF_9;
        else if (payload <= mDot::MaxLengths_915[mDot::SF_8])
            return mDot::SF_8;
        else
            return mDot::SF_7;
    } else {
        if (payload <= mDot::MaxLengths_868[mDot::SF_12])
            return mDot::SF_12;
        else if (payload <= mDot::MaxLengths_868[mDot::SF_9])
            return mDot::SF_9;
        else
            return mDot::SF_7;
    }

    return mDot::SF_7;
}

