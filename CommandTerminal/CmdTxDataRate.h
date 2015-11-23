#ifndef __CMDTXDATARATE_H__
#define __CMDTXDATARATE_H__

#include "Command.h"

class ModeConfig;

class CmdTxDataRate : public Command {

public:

    CmdTxDataRate(mDot* dot, mts::MTSSerial& serial);
    virtual uint32_t action(std::vector<std::string> args);
    virtual bool verify(std::vector<std::string> args);
    
private:   
    mts::MTSSerial& _serial;
};

#endif // __CMDTXDATARATE_H__
