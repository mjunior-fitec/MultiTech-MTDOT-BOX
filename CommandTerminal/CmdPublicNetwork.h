#ifndef __CMDPUBLICNETWORK_H__
#define __CMDPUBLICNETWORK_H__

#include "Command.h"

class CommandTerminal;

class CmdPublicNetwork : public Command {

public:

    CmdPublicNetwork(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   

    mts::MTSSerial& _serial;
};

#endif // __CMDPUBLICNETWORK_H__
