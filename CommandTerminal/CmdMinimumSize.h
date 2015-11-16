#ifndef __CMDMINIMUMSIZE_H__
#define __CMDMINIMUMSIZE_H__

#include "Command.h"

class CommandTerminal;

class CmdMinimumSize: public Command
{

public:

    CmdMinimumSize(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);

private:
    mts::MTSSerial& _serial;
};

#endif // __CMDMINIMUMSIZE_H__
