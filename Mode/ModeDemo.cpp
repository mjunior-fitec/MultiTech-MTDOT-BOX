#include "ModeDemo.h"
#include "MTSLog.h"

// 10 s, 30 s, 1 min, 5 min, 10 min, 15 min, 30 min 1 hour
const uint32_t ModeDemo::_intervals[] = { 10, 30, 60, 5 * 60, 10 * 60, 15 * 60, 30 * 60, 60 * 60 };

ModeDemo::ModeDemo(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors)
  : Mode(lcd, buttons, dot, lora, gps, sensors),
    _help(lcd),
    _sam(lcd),
    _interval(0)
{}

ModeDemo::~ModeDemo() {}

bool ModeDemo::start() {
    bool send = false;
    bool no_channel = false;

    // clear any stale signals
    osSignalClear(_main_id, buttonSignal | loraSignal);

    _state = show_help;
    displayHelp();

    while (true) {
        osEvent e = Thread::signal_wait(0, 250);
        if (e.status == osEventSignal) {
            if (e.value.signals & buttonSignal) {
                _be = _buttons->getButtonEvent();

                switch (_be) {
                    case ButtonHandler::sw1_press:
                        switch (_state) {
                            case show_help:
                                _state = sampling;
                                _mode = trigger;
                                _sam.display();
                                _sam.updateSw2("Send");
                                break;
                            case sampling:
                                if (_mode == interval) {
                                    _interval = (_interval + 1) % (sizeof(_intervals) / sizeof(uint32_t));
                                    _sam.updateInterval(_intervals[_interval]);
                                }
                                break;
                        }
                        break;

                    case ButtonHandler::sw2_press:
                        switch (_state) {
                            case show_help:
                                _state = sampling;
                                _mode = interval;
                                _send_timer.start();
                                _sam.display();
                                _sam.updateSw1("Interval");
                                _sam.updateInterval(_intervals[_interval]);
                                break;
                            case sampling:
                                if (_mode == trigger) {
                                    if (_dot->getNextTxMs() > 0)
                                        no_channel = true;
                                    else
                                        send = true;
                                }
                                break;
                        }
                        break;
                    case ButtonHandler::sw1_hold:
                        _send_timer.stop();
                        _send_timer.reset();
                        return true;
                }
            }
            if (e.value.signals & loraSignal) {
                _ls = _lora->getStatus();
                switch (_ls) {
                    case LoRaHandler::send_success:
                        switch (_state) {
                            case sampling:
                                if (_mode == trigger) {
                                    _sam.updateSw2("Send");
                                    _sam.updateInfo("                 ");
                                } else {
                                    _sam.updateSw1("Interval");
                                    _sam.updateInterval(_intervals[_interval]);
                                }
                                break;
                        }
                        break;

                    case LoRaHandler::send_failure:
                        switch (_state) {
                            case sampling:
                                if (_mode == trigger) {
                                    _sam.updateSw2("Send");
                                    _sam.updateInfo("                 ");
                                } else {
                                    _sam.updateSw1("Interval");
                                    _sam.updateInterval(_intervals[_interval]);
                                }
                                break;
                        }
                        break;
                }
            }
        }

        if (_send_timer.read_ms() > _intervals[_interval] * 1000) {
            _send_timer.reset();
            if (_dot->getNextTxMs() > 0)
                no_channel = true;
            else
                send = true;
        }
        if (no_channel) {
            uint32_t t = _dot->getNextTxMs();
            if (t > 0) {
                logInfo("next tx %lu ms", t);
                _sam.updateCountdown(t / 1000);
            } else {
                no_channel = false;
                send = true;
            }
        }
        if (send) {
            std::vector<uint8_t> s_data = formatSensorData(_data);
            logInfo("sending data %s %d", _dot->DataRateStr(_dot->getTxDataRate()).c_str(), _dot->getTxPower());
            _sam.updateInfo("Sending...");
            _sam.updateSw1("        ");
            _sam.updateSw2("        ");
            send = false;
            // we don't care if the server actually gets this packet or not
            // we won't retry anyway
            _dot->setAck(0);
            _dot->setTxWait(false);
            _lora->send(s_data);
            osDelay(500);
        }
        if(_state != show_help){
            updateSensorData(_data);
            _sam.updateAccelerationX(_data.accel_data._x);
            _sam.updateAccelerationY(_data.accel_data._y);
            _sam.updateAccelerationZ(_data.accel_data._z);
    		_sam.updatePressure(_data.pressure);
    		_sam.updateAltitude(_data.altitude);
    		_sam.updateTemperature(_data.temperature);
    		_sam.updateLight(_data.light);        
        }
    }
}

void ModeDemo::displayHelp() {
    _help.display();
    _help.updateMode("LoRa Demo");
    _help.updateDescription("Select TX Method");
    _help.updateSw1(" Trigger");
    _help.updateSw2("Interval");
}

