#ifndef __CMDFREQUENCYSUBBAND_H__
#define __CMDFREQUENCYSUBBAND_H__

#include "Command.h"

class ModeConfig;

class CmdFrequencySubBand : public Command {

public:

    CmdFrequencySubBand(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   
    mts::MTSSerial& _serial;
};

#endif // __CMDFREQUENCYSUBBAND_H__
