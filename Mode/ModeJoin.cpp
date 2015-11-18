#include "ModeJoin.h"
#include "MTSLog.h"
#include "MTSText.h"

ModeJoin::ModeJoin(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, uint8_t band)
  : Mode(lcd, buttons),
    _lj(lcd, band),
    _dot(dot),
    _lora(lora),
    _band(band),
    _data_rate(band == mDot::FB_915 ? mDot::SF_10 : mDot::SF_12),
    _power(20),
    _next_tx(0),
    _joined(false)
{}

ModeJoin::~ModeJoin() {}

bool ModeJoin::start() {
    // clear any stale signals
    osSignalClear(_main_id, buttonSignal | loraSignal);

    _lj.display();
    _lj.updateStatus("Joining...");
    if (_dot->getJoinMode() == mDot::MANUAL) {
        _lj.updateId(mts::Text::bin2hexString(_dot->getNetworkId()));
        _lj.updateKey(mts::Text::bin2hexString(_dot->getNetworkKey()));
    } else {
        _lj.updateId(_dot->getNetworkName());
        _lj.updateKey(_dot->getNetworkPassphrase());
    }
    if (_band == mDot::FB_915) {
        _sub_band = _dot->getFrequencySubBand();
        _lj.updateFsb(_sub_band);
    }
    // mDot::DataRateStr returns format SF_XX - we only want to display the XX part
    _lj.updateRate(_dot->DataRateStr(_data_rate).substr(3));
    _lj.updatePower(_power);

    _lora->setDataRate(_data_rate);
    _lora->setPower(_power);

    while (! _joined) {
        _next_tx = _lora->getNextTx();
        if (_next_tx) {
            _lj.updateCountdown(_next_tx * 1000);
        } else {
            _lj.updateAttempt(++_index);
            _lj.updateStatus("Joining...");
            _lora->join();
        }

        osEvent e = Thread::signal_wait(0);
        if (e.status == osEventSignal) {
            if (e.value.signals & buttonSignal) {
                _be = _buttons->getButtonEvent();
                switch (_be) {
                    case ButtonHandler::sw1_press:
                        return false;
                    case ButtonHandler::sw2_press:
                        break;
                    case ButtonHandler::sw1_hold:
                        return false;
                }
            }
            if (e.value.signals & loraSignal) {
                _ls = _lora->getStatus();
                switch (_ls) {
                    case LoRaHandler::join_success:
                        _lj.updateStatus("Join Success!");
                        _lj.displayCancel(false);
                        logInfo("joined");
                        _joined = true;
                        osDelay(2000);
                        return true;

                    case LoRaHandler::join_failure:
                        logInfo("failed to join");
                        break;
                }
            }
        }
    }

    return false;
}
