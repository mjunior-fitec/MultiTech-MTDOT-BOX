#ifndef __CMDTXPOWER_H__
#define __CMDTXPOWER_H__

#include "Command.h"

class CommandTerminal;

class CmdTxPower: public Command
{

public:

    CmdTxPower(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);

private:
    mts::MTSSerial& _serial;
};

#endif // __CMDTXPOWER_H__
