
#ifndef __CMDNETWORKKEY_H__
#define __CMDNETWORKKEY_H__


#include "Command.h"

class ModeConfig;

class CmdNetworkKey : public Command {

public:

    CmdNetworkKey(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   
    mts::MTSSerial& _serial;
};

#endif // __CMDNETWORKKEY_H__
