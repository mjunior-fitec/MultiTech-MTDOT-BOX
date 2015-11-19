#include "ModeConfig.h"
#include "MTSLog.h"
#include "MTSText.h"
#include "CommandTerminal.h"

ModeConfig::ModeConfig(DOGS102* lcd, ButtonHandler* buttons, mDot* dot)
  : Mode(lcd, buttons),
    _lc(lcd),
    _dot(dot)
{}

ModeConfig::~ModeConfig() {}

bool ModeConfig::start() {
    // clear any stale signals
    osSignalClear(_main_id, buttonSignal);

    _lc.display();

    logInfo("config mode");

	mts::MTSSerial serial(USBTX, USBRX, 512, 512);

    CommandTerminal term(serial, _dot, _buttons);
   	return term.start();		
}	
