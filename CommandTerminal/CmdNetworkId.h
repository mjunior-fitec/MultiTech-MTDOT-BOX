#ifndef __CMDNETWORKID_H__
#define __CMDNETWORKID_H__

#include "Command.h"

class ModeConfig;

class CmdNetworkId : public Command {

public:

    CmdNetworkId(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   
    mts::MTSSerial& _serial;
};

#endif // __CMDNETWORKID_H__
