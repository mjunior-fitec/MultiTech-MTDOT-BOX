#include "CmdExit.h"

CmdExit::CmdExit(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Exit", "AT+EXIT", "Exit from command mode to main menu"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
}

uint32_t CmdExit::action(std::vector<std::string> args)
{
//ToDo: Probably setting or clearing a flag to indicate to some other task to shut down command mode and display the main menu.
	if (!_dot->saveConfig()) {
	  setErrorMessage("Failed to save to flash");
	  return 1;
	}
	
	return 0;

}
