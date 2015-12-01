#include "ModeSingle.h"
#include "MTSLog.h"

ModeSingle::ModeSingle(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora)
  : Mode(lcd, buttons, dot, lora),
    _help(lcd),
    _file(lcd),
    _confirm(lcd),
    _progress(lcd),
    _success(lcd),
    _failure(lcd)
{}

ModeSingle::~ModeSingle() {}

bool ModeSingle::start() {
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

    // see if survey data file exists
    std::vector<mDot::mdot_file> files = _dot->listUserFiles();
    for (std::vector<mDot::mdot_file>::iterator it = files.begin(); it != files.end(); it++) {
        if (strcmp(it->name, _file_name) == 0) {
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
        _index = 0;
        displayHelp();
    }

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
                                _index = getIndex(single);
                                displayHelp();
                                break;
                            case confirm:
                                _state = check_file;
                                _file.display();
                                break;
                            case show_help:
                                incrementRatePower();
                                _help.updateMsg(formatNewRatePower());
                                logInfo("new data rate %u, power %lu", _data_rate, _power);
                                break;
                            case in_progress:
                                // do nothing
                                break;
                            case success:
                                incrementRatePower();
                                _success.updateInfo(formatNewRatePower());
                                logInfo("new data rate %u, power %lu", _data_rate, _power);
                                break;
                            case data:
                                break;
                            case failure:
                                incrementRatePower();
                                _failure.updateInfo(formatNewRatePower());
                                logInfo("new data rate %u, power %lu", _data_rate, _power);
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
                                _dot->deleteUserFile(_file_name);
                                _index = 0;
                                displayHelp();
                                break;
                            case show_help:
                                _state = in_progress;
                                _progress.display();
                                if (_lora->getNextTx() > 0)
                                    no_channel_ping = true;
                                else 
                                    send_ping = true;
                                break;
                            case in_progress:
                                // do nothing
                                break;
                            case success:
                                _state = in_progress;
                                _progress.display();
                                if (_lora->getNextTx() > 0)
                                    no_channel_ping = true;
                                else 
                                    send_ping = true;
                                break;
                            case data:
                                break;
                            case failure:
                                _state = in_progress;
                                _progress.display();
                                if (_lora->getNextTx() > 0)
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
                                _ping_result = _lora->getPingResults();
                                displaySuccess();
                                logInfo("ping successful");
                                updateData(_data, single, true);
                                appendDataFile(_data);
                                if (_send_data) {
                                    _state = data;
                                    if (_lora->getNextTx() > 0)
                                        no_channel_data = true;
                                    else
                                        send_data = true;
                                } else {
                                    _state = success;
                                    _success.updateSw1("   Power");
                                    _success.updateSw2("Survey");
                                }
                                break;
                            case success:
                                break;
                            case data:
                                break;
                            case failure:
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
                                _state = failure;
                                _failure.display();
                                _failure.updateId(_index);
                                // mDot::DataRateStr returns format SF_XX - we only want to display the XX part
                                _failure.updateRate(_dot->DataRateStr(_data_rate).substr(3));
                                updateData(_data, single, false);
                                appendDataFile(_data);
                                _failure.updatePower(_power);
                                logInfo("ping failed");
                                break;
                            case success:
                                break;
                            case data:
                                break;
                            case failure:
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
                                _success.updateInfo("Data Send Success");
                                _success.updateSw1("   Power");
                                _success.updateSw2("Survey");
                                logInfo("data send success");
                                break;
                            case failure:
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
                                _success.updateInfo("Data Send Failure");
                                _success.updateSw1("   Power");
                                _success.updateSw2("Survey");
                                logInfo("data send failed");
                                break;
                            case failure:
                                break;
                        }
                        break;
                }
            }
        }

        if (no_channel_ping) {
            uint32_t t = _lora->getNextTx();
            if (t > 0) {
                logInfo("next tx %lu ms", t);
                _progress.updateCountdown(t / 1000);
            } else {
                _progress.display();
                no_channel_ping = false;
                send_ping = true;
            }
        }
        if (no_channel_data) {
            uint32_t t = _lora->getNextTx();
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
            logInfo("sending ping %s %d", _dot->DataRateStr(_data_rate).c_str(), _power);
            send_ping = false;
            _lora->setDataRate(_data_rate);
            _lora->setPower(_power);
            _lora->ping();
            _index++;
        }
        if (send_data) {
            std::vector<uint8_t> s_data;
            logInfo("sending data %s %d", _dot->DataRateStr(_data_rate).c_str(), _power);
            send_data = false;
            _success.updateInfo("Data Sending...");
            _lora->setDataRate(_data_rate);
            _lora->setPower(_power);
            _lora->send(s_data);
        }
    }
}

void ModeSingle::displayHelp() {
    _help.display();
    _help.updateMode("Survey Single");
    _help.updateSw1("  DR/PWR");
    _help.updateSw2("Survey");
}

void ModeSingle::displaySuccess() {
    _success.display();
    _success.updateId(_index);
    // mDot::DataRateStr returns format SF_XX - we only want to display the XX part
    _success.updateRate(_dot->DataRateStr(_data_rate).substr(3));
    _success.updatePower(_power);
    _success.updateStats(_ping_result);
    // if GPS lock
    // display GPS latitude, longitude, and time
    // else
    // display "no lock"
    _success.updateInfo("No GPS Lock");
}

std::string ModeSingle::formatNewRatePower() {
    std::string msg;
    char buf[8];
    size_t size;

    memset(buf, 0, sizeof(buf));
    msg += "New DR=";
    msg += _dot->DataRateStr(_data_rate).substr(3);
    msg += " P=";
    size = snprintf(buf, sizeof(buf), "%u", _power);
    msg.append(buf, size);

    return msg;
}

void ModeSingle::incrementRatePower() {
    if (_power == 20) {
        _power = 2;
        switch (_data_rate) {
            case mDot::SF_7:
                _data_rate = mDot::SF_8;
                break;
            case mDot::SF_8:
                _data_rate = mDot::SF_9;
                break;
            case mDot::SF_9:
                _data_rate = mDot::SF_10;
                break;
            case mDot::SF_10:
                if (_band == mDot::FB_915)
                    _data_rate = mDot::SF_7;
                else
                    _data_rate = mDot::SF_11;
                break;
            case mDot::SF_11:
                _data_rate = mDot::SF_12;
                break;
            case mDot::SF_12:
                _data_rate = mDot::SF_7;
                break;
        }
    } else {
        _power += 3;
    }
}

