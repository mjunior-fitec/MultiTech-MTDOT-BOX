#ifndef __CMDDELETESURVEYDATAFILE_H__
#define __CMDDELETESURVEYDATAFILE_H__

#include "Command.h"

class ModeConfig;

class CmdDeleteSurveyDataFile: public Command
{

public:

    CmdDeleteSurveyDataFile(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);

private:
    mts::MTSSerial& _serial;
};

#endif // __CMDDELETESURVEYDATAFILE_H__
