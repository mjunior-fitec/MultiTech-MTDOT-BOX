#ifndef __CMDDEVICEID_H__
#define __CMDDEVICEID_H__

#include "Command.h"

class CommandTerminal;

class CmdDeviceId : public Command {

public:

    CmdDeviceId(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   

    mts::MTSSerial& _serial;
};

#endif // __CMDDEVICEID_H__
