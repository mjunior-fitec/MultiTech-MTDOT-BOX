#ifndef __CMDNETWORKJOINMODE_H__
#define __CMDNETWORKJOINMODE_H__

#include "Command.h"

class ModeConfig;

class CmdNetworkJoinMode : public Command {

public:

    CmdNetworkJoinMode(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   
    mts::MTSSerial& _serial;
};

#endif // __CMDNETWORKJOINMODE_H__
