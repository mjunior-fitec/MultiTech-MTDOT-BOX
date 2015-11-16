#ifndef __CMDMINIMUMPOWER_H__
#define __CMDMINIMUMPOWER_H__

#include "Command.h"

class CommandTerminal;

class CmdMinimumPower: public Command
{

public:

    CmdMinimumPower(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);

private:
    mts::MTSSerial& _serial;
};

#endif // __CMDMINIMUMPOWER_H__
