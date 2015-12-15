#include "ModeSingle.h"
#include "MTSLog.h"

ModeSingle::ModeSingle(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors)
  : Mode(lcd, buttons, dot, lora, gps, sensors),
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
                                _help.updateMsg(formatRatePower());
                                break;
                            case success:
                                incrementRatePower();
                                _success.updateInfo(formatRatePower());
                                break;
                            case failure:
                                incrementRatePower();
                                _failure.updateInfo(formatRatePower());
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
                                _index = 0;
                                displayHelp();
                                break;
                            case show_help:
                                _state = in_progress;
                                _progress.display();
                                if (_dot->getNextTxMs() > 0)
                                    no_channel_link_check = true;
                                else 
                                    send_link_check = true;
                                break;
                            case success:
                                _state = in_progress;
                                _progress.display();
                                if (_dot->getNextTxMs() > 0)
                                    no_channel_link_check = true;
                                else 
                                    send_link_check = true;
                                break;
                            case failure:
                                _state = in_progress;
                                _progress.display();
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
                                _link_check_result = _lora->getLinkCheckResults();
                                displaySuccess();
                                logInfo("link check successful\tMargin %ld\tRSSI %d dBm\tSNR %2.3f", _link_check_result.up.dBm, _link_check_result.down.rssi, (float)_link_check_result.down.snr / 10.0);
                                updateData(_data, single, true);
                                appendDataFile(_data);
                                if (_send_data) {
                                    _state = data;
                                    if (_dot->getNextTxMs() > 0)
                                        no_channel_data = true;
                                    else
                                        send_data = true;
                                } else {
                                    _state = success;
                                    _success.updateSw1("  DR/PWR");
                                    _success.updateSw2("Survey");
                                }
                                break;
                        }
                        break;

                    case LoRaHandler::link_check_failure:
                        switch (_state) {
                            case in_progress:
                                _state = failure;
                                _failure.display();
                                _failure.updateId(_index);
                                // mDot::DataRateStr returns format SF_XX - we only want to display the XX part
                                _failure.updateRate(_dot->DataRateStr(_data_rate).substr(3));
                                updateData(_data, single, false);
                                appendDataFile(_data);
                                _failure.updatePower(_power);
                                if (_gps_available && _gps->getLockStatus()) {
                                    GPSPARSER::latitude lat = _gps->getLatitude();
                                    GPSPARSER::longitude lon = _gps->getLongitude();
                                    struct tm time = _gps->getTimestamp();
                                    _failure.updateGpsLatitude(lat);
                                    _failure.updateGpsLongitude(lon);
                                    _failure.updateGpsTime(time);
                                } else {
                                    _failure.updateGpsLatitude("No GPS Lock");
                                }
                                _failure.updateSw1("  DR/PWR");
                                _failure.updateSw2("Survey");
                                logInfo("link check failed");
                                break;
                        }
                        break;

                    case LoRaHandler::send_success:
                        switch (_state) {
                            case data:
                                _state = success;
                                _success.updateInfo("                 ");
                                _success.updateSw1("  DR/PWR");
                                _success.updateSw2("Survey");
                                // turn acks and receive windows back on
                                _dot->setAck(1);
                                _dot->setTxWait(true);
                                logInfo("data send success");
                                break;
                        }
                        break;

                    case LoRaHandler::send_failure:
                        switch (_state) {
                            case data:
                                _state = success;
                                _success.updateInfo("                 ");
                                _success.updateSw1("  DR/PWR");
                                _success.updateSw2("Survey");
                                // turn acks and receive windows back on
                                _dot->setAck(1);
                                _dot->setTxWait(true);
                                logInfo("data send failed");
                                break;
                        }
                        break;
                }
            }
        }

        if (no_channel_link_check) {
            uint32_t t = _dot->getNextTxMs();
            if (t > 0) {
                logInfo("next tx %lu ms", t);
                _progress.updateCountdown(t / 1000);
            } else {
                _progress.display();
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
            logInfo("sending link check %s %d", _dot->DataRateStr(_data_rate).c_str(), _power);
            send_link_check = false;
            _dot->setTxDataRate(_data_rate);
            _dot->setTxPower(_power);
            _lora->linkCheck();
            _index++;
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

void ModeSingle::displayHelp() {
    _help.display();
    _help.updateMode("Survey Single");
    _help.updateSw1("  DR/PWR");
    _help.updateSw2("Survey");
    _help.updateMsg(formatRatePower());
}

void ModeSingle::displaySuccess() {
    uint8_t fix = _gps->getFixStatus();
    _success.display();
    _success.updateId(_index);
    // mDot::DataRateStr returns format SF_XX - we only want to display the XX part
    _success.updateRate(_dot->DataRateStr(_data_rate).substr(3));
    _success.updatePower(_power);
    _success.updateStats(_link_check_result);
    if (_gps_available && _gps->getLockStatus()) {
        GPSPARSER::latitude lat = _gps->getLatitude();
        GPSPARSER::longitude lon = _gps->getLongitude();
        struct tm time = _gps->getTimestamp();
        _success.updateGpsLatitude(lat);
        _success.updateGpsLongitude(lon);
        _success.updateGpsTime(time);
    } else {
        _success.updateGpsLatitude("No GPS Lock");
    }
}

std::string ModeSingle::formatRatePower() {
    std::string msg;
    char buf[8];
    size_t size;

    msg += "DR=";
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

    logInfo("new data rate %s, power %lu", mDot::DataRateStr(_data_rate).c_str(), _power);
}

