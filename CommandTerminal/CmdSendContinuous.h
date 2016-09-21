
#ifndef __CMDSENDCONTINUOUS_H__
#define __CMDSENDCONTINUOUS_H__

#include "Command.h"

class CmdSendContinuous : public Command {

public:

    CmdSendContinuous(mDot* dot);
    uint32_t action(std::vector<std::string> args);
    
private:
    
};

#endif // __CMDSENDCONTINUOUS_H__
