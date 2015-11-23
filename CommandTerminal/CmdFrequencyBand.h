#ifndef __CMDFREQUENCYBAND_H__
#define __CMDFREQUENCYBAND_H__

#include "Command.h"

class CommandTerminal;

class CmdFrequencyBand : public Command {

public:

    CmdFrequencyBand(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   

    mts::MTSSerial& _serial;
};

#endif // __CMDFREQUENCYBAND_H__
