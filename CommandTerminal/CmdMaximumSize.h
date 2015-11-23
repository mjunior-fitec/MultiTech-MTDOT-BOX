#ifndef __CMDMAXIMUMSIZE_H__
#define __CMDMAXIMUMSIZE_H__

#include "Command.h"

class ModeConfig;

class CmdMaximumSize: public Command
{

public:

    CmdMaximumSize(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);

private:
    mts::MTSSerial& _serial;
};

#endif // __CMDMAXIMUMSIZE_H__
