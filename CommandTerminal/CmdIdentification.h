#ifndef __CMDIDENTIFICATION_H__
#define __CMDIDENTIFICATION_H__

#include "Command.h"

class CmdIdentification : public Command {

public:

    CmdIdentification(mDot* dot, mts::MTSSerial& serial);   
    virtual uint32_t action(std::vector<std::string> args);
    
private:
    mts::MTSSerial& _serial;
};

#endif // __CMDIDENTIFICATION_H__
