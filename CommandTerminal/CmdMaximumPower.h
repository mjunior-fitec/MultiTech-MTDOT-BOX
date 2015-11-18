#ifndef __CMDMAXIMUMPOWER_H__
#define __CMDMAXIMUMPOWER_H__

#include "Command.h"

class CommandTerminal;

class CmdMaximumPower: public Command
{

public:

    CmdMaximumPower(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);

private:
    mts::MTSSerial& _serial;
};

#endif // __CMDMAXIMUMPOWER_H__
