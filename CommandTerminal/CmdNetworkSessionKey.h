#ifndef __CMDNETWORKSESSIONKEY_H__
#define __CMDNETWORKSESSIONKEY_H__

#include "Command.h"

class ModeConfig;

class CmdNetworkSessionKey : public Command {

public:

    CmdNetworkSessionKey(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   
    mts::MTSSerial& _serial;
};

#endif // __CMDNETWORKSESSIONKEY_H__
