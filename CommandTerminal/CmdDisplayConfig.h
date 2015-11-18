#ifndef __CMDDISPLAYCONFIG_H__
#define __CMDDISPLAYCONFIG_H__

#include "Command.h"

class CmdDisplayConfig : public Command {

public:

    CmdDisplayConfig(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);

private:

    mts::MTSSerial& _serial;

};

#endif // __CMDDISPLAYCONFIG_H__
