#ifndef __CMDNETWORKADDRESS_H__
#define __CMDNETWORKADDRESS_H__

#include "Command.h"

class ModeConfig;

class CmdNetworkAddress : public Command {

public:

    CmdNetworkAddress(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   
    mts::MTSSerial& _serial;
};

#endif // __CMDNETWORKADDRESS_H__
