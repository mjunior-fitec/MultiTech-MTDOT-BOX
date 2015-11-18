#include "CmdDeleteSurveyDataFile.h"

CmdDeleteSurveyDataFile::CmdDeleteSurveyDataFile(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Delete Survey Data File", "AT+DSDF", "Delete the survey data file"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
}

uint32_t CmdDeleteSurveyDataFile::action(std::vector<std::string> args)
{
//ToDo: Delete the file.
	if (!_dot->saveConfig()) {
	  setErrorMessage("Failed to save to flash");
	  return 1;
	}
	
	return 0;

}


