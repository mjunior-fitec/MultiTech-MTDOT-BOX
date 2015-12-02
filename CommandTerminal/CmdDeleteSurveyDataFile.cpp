#include "CmdDeleteSurveyDataFile.h"
#include "FileName.h"

CmdDeleteSurveyDataFile::CmdDeleteSurveyDataFile(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Delete Survey Data File", "AT+DSDF", "Delete the survey data file"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
}

uint32_t CmdDeleteSurveyDataFile::action(std::vector<std::string> args)
{
    if (!_dot->deleteUserFile(file_name)) {
	setErrorMessage("Failed to delete survey data file");
	return 1;
    }

    return 0;
}


