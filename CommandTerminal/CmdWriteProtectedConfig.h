
#ifndef __CMDWRITEPROTECTEDCONFIG_H__
#define __CMDWRITEPROTECTEDCONFIG_H__

#include "Command.h"

class CmdWriteProtectedConfig : public Command {

public:

    CmdWriteProtectedConfig(mDot* dot);
    virtual uint32_t action(std::vector<std::string> args);

private:
};

#endif // __CMDWRITEPROTECTEDCONFIG_H__
