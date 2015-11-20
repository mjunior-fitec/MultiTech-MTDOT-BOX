#ifndef __CMDDATA_H__
#define __CMDDATA_H__

#include "Command.h"

class ModeConfig;

class CmdData: public Command
{

public:

    CmdData(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);

private:
    mts::MTSSerial& _serial;
};

#endif // __CMDDATA_H__
