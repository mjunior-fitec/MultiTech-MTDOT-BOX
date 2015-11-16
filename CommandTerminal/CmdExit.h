#ifndef __CMDEXIT_H__
#define __CMDEXIT_H__

#include "Command.h"

class CommandTerminal;

class CmdExit: public Command
{

public:

    CmdExit(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);

private:
    mts::MTSSerial& _serial;
};

#endif // __CMDEXIT_H__
