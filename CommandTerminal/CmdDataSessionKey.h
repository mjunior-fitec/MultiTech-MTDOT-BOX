#ifndef __CMDDATASESSIONKEY_H__
#define __CMDDATASESSIONKEY_H__

#include "Command.h"

class ModeConfig;

class CmdDataSessionKey : public Command {

public:

    CmdDataSessionKey(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   
    mts::MTSSerial& _serial;
};

#endif // __CMDDATASESSIONKEY_H__
