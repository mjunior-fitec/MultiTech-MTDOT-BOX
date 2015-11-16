#include "CmdGetSurveyDataFile.h"

CmdGetSurveyDataFile::CmdGetSurveyDataFile(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Get Survey Data File", "AT+GSDF", "Outputs the survey data file to the command port"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
}

uint32_t CmdGetSurveyDataFile::action(std::vector<std::string> args)
{
//ToDo: Output the file contents line by line.
    if (!_dot->saveConfig()) {
      setErrorMessage("Failed to save to flash");
      return 1;
    }
    
    return 0;

}
