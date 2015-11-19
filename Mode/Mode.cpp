#include "Mode.h"

const char* Mode::_file_name = "SurveyData.txt";

Mode::Mode(DOGS102* lcd, ButtonHandler* buttons)
  : _lcd(lcd),
    _buttons(buttons),
    _index(0),
    _main_id(Thread::gettid())
{}

Mode::~Mode() {}

bool Mode::deleteDataFile() {
    return true;
}

bool Mode::appendDataFile(const DataItem& data) {
    return true;
}

