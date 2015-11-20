#include "Mode.h"

const char* Mode::_file_name = "SurveyData.txt";

Mode::Mode(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora)
  : _lcd(lcd),
    _buttons(buttons),
    _dot(dot),
    _lora(lora),
    _main_id(Thread::gettid()),
    _index(0),
    _band(_dot->getFrequencyBand()),
    _sub_band(0),
    _data_rate(mDot::SF_7),
    _power(2),
    _next_tx(0),
    _send_data(false)
{}

Mode::~Mode() {}

bool Mode::deleteDataFile() {
    return true;
}

bool Mode::appendDataFile(const DataItem& data) {
    return true;
}

