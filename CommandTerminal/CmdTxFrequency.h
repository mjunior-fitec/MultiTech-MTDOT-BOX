#ifndef __CMDTXFREQUENCY_H__
#define __CMDTXFREQUENCY_H__

#include "Command.h"

class CommandTerminal;

class CmdTxFrequency : public Command {

public:

    CmdTxFrequency(mDot* dot);
    uint32_t action(std::vector<std::string> args);
    bool verify(std::vector<std::string> args);
    
private:   
    
};

#endif // __CMDTXFREQUENCY_H__
