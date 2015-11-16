#ifndef __CMDGETSURVEYDATAFILE_H__
#define __CMDGETSURVEYDATAFILE_H__

#include "Command.h"

class CommandTerminal;

class CmdGetSurveyDataFile: public Command
{

public:

    CmdGetSurveyDataFile(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);

private:
    mts::MTSSerial& _serial;
};

#endif // __CMDGETSURVEYDATAFILE_H__
